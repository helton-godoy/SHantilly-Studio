#include "PreviewManager.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QTemporaryFile>

PreviewManager::PreviewManager(QObject* parent) : QObject(parent) {
}

void PreviewManager::runPreview(const QString& scriptContent, const QString& executablePath) {
    QTemporaryFile tempFile;
    if (!tempFile.open()) {
        emit previewError("Failed to create temporary file for preview.");
        return;
    }

    tempFile.write(scriptContent.toUtf8());
    tempFile.flush();
    tempFile.setAutoRemove(false);
    QString tempPath = tempFile.fileName();
    tempFile.close();

    QProcess* process = new QProcess(this);

    // Use the provided executable path
    QString absPath = executablePath;
    if (absPath.isEmpty()) {
         // Fallback default for development convenience
         absPath = "../SHantilly/build/bin/shantilly";
    }

    // Verify absolute path resolution from current working directory just in case relative path is provided
    if (QFileInfo(absPath).isRelative()) {
        absPath = QDir::current().absoluteFilePath(absPath);
    }

    if (!QFile::exists(absPath)) {
        emit previewError("Executable not found at: " + absPath + "\nPlease configure the SHantilly executable path in Edit -> Preferences.");
        QFile::remove(tempPath);
        process->deleteLater();
        return;
    }

    connect(process, &QProcess::readyReadStandardOutput, this,
            [this, process]() { emit previewOutput(process->readAllStandardOutput()); });

    connect(process, &QProcess::readyReadStandardError, this,
            [this, process]() { emit previewError(process->readAllStandardError()); });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [this, process, tempPath](int exitCode, QProcess::ExitStatus status) {
                if (status == QProcess::CrashExit) {
                    emit previewError("Process Crashed (Segmentation Fault or Aborted).");
                    emit previewFinished(139); // Typical SIGSEGV code
                } else {
                    emit previewFinished(exitCode);
                }

                QFile::remove(tempPath);
                process->deleteLater();
            });

    // poc_modern_cli reads from stdin
    process->setStandardInputFile(tempPath);
    // Remove --keep-open as it is not supported by the main shantilly executable
    process->start(absPath, QStringList());
}
