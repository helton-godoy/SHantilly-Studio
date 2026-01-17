#include "MainWindow.h"
#include "Canvas.h"
#include "ObjectInspector.h"
#include "PropertyEditor.h"
#include "core/StudioWidgetFactory.h"
#include "core/StudioController.h"
#include "core/StudioCommands.h"
#include "core/ScriptGenerator.h"
#include "core/ProjectSerializer.h"
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QListWidget>
#include <QToolBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Showbox Studio");
    resize(1200, 800);
    
    m_factory = new StudioWidgetFactory();
    m_controller = new StudioController(this);
    
    setupUI();
    m_canvas->setController(m_controller);
    m_propEditor->setController(m_controller);
    createSampleWidgets();

    // Sincronizar UI com Undo/Redo
    connect(m_controller->undoStack(), &QUndoStack::indexChanged, [this]() {
        if (m_controller->selectedWidget()) {
            m_propEditor->setTargetWidget(m_controller->selectedWidget());
        }
    });
    
    // Sincronizar seleção: Canvas -> Inspector & Property Editor

    // MenuBar
    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("Open", this, &MainWindow::onOpenClicked, QKeySequence::Open);
    fileMenu->addAction("Save", this, &MainWindow::onSaveClicked, QKeySequence::Save);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close, QKeySequence::Quit);

    QMenu *editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction(m_controller->undoStack()->createUndoAction(this));
    editMenu->addAction(m_controller->undoStack()->createRedoAction(this));
    
    // Sincronizar seleção: Canvas -> Inspector & Property Editor
    connect(m_controller, &StudioController::widgetSelected, m_inspector, &ObjectInspector::selectItemForWidget);
    connect(m_controller, &StudioController::widgetSelected, m_propEditor, &PropertyEditor::setTargetWidget);
    
    // Conectar novos widgets do Canvas ao Controller e atualizar árvore
    connect(m_canvas, &Canvas::widgetAdded, m_controller, &StudioController::manageWidget);
    connect(m_canvas, &Canvas::widgetSelected, m_controller, &StudioController::selectWidget); // Novo
    connect(m_canvas, &Canvas::widgetAdded, [this]() {
        m_inspector->updateHierarchy(m_canvas);
    });

    // Bidirecional: Inspector -> Controller
    connect(m_inspector, &ObjectInspector::itemSelected, m_controller, &StudioController::selectWidget);
    
    // Atualizar hierarquia inicial
    m_inspector->updateHierarchy(m_canvas);
}

MainWindow::~MainWindow()
{
    delete m_factory;
}

void MainWindow::setupUI()
{
    statusBar()->showMessage("Pronto");

    // Toolbar
    QToolBar *toolBar = addToolBar("Main Toolbar");
    
    QAction *openAction = toolBar->addAction("Open");
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpenClicked);

    QAction *saveAction = toolBar->addAction("Save");
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveClicked);

    toolBar->addSeparator();

    // Undo/Redo Actions (via QUndoStack)
    QAction *undoAction = m_controller->undoStack()->createUndoAction(this, "Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setIcon(QIcon::fromTheme("edit-undo"));
    toolBar->addAction(undoAction);

    QAction *redoAction = m_controller->undoStack()->createRedoAction(this, "Redo");
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setIcon(QIcon::fromTheme("edit-redo"));
    toolBar->addAction(redoAction);

    toolBar->addSeparator();

    QAction *runAction = toolBar->addAction("Run Preview");
    runAction->setShortcut(QKeySequence("F5"));
    connect(runAction, &QAction::triggered, this, &MainWindow::onRunClicked);

    // Atalho global para Delete
    QAction *deleteAction = new QAction(this);
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::onDeleteClicked);
    addAction(deleteAction);

    // Central Widget: CANVAS
    m_canvas = new Canvas(m_factory, this);
    setCentralWidget(m_canvas);

    // Left Dock: TOOLBOX & INSPECTOR (em abas)
    setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::North);

    QDockWidget *dockInspector = new QDockWidget("Hierarquia", this);
    m_inspector = new ObjectInspector(dockInspector);
    dockInspector->setWidget(m_inspector);
    addDockWidget(Qt::LeftDockWidgetArea, dockInspector);

    QDockWidget *dockToolbox = new QDockWidget("Toolbox", this);
    QToolBox *toolBoxWidget = new QToolBox(dockToolbox);
    
    // Helper lambda para criar listas de categorias
    auto createCategory = [&](const QStringList &items) -> QListWidget* {
        QListWidget *list = new QListWidget();
        list->setDragEnabled(true);
        list->setDragDropMode(QAbstractItemView::DragOnly);
        list->addItems(items);
        return list;
    };

    // 1. Controles Básicos
    toolBoxWidget->addItem(createCategory({
        "Label", "PushButton", "CheckBox", "RadioButton", "Separator"
    }), "Básico");

    // 2. Entrada de Dados
    toolBoxWidget->addItem(createCategory({
        "LineEdit", "TextEdit", "SpinBox", "Slider", "ComboBox", "Calendar"
    }), "Entrada");

    // 3. Dados & Visualização
    toolBoxWidget->addItem(createCategory({
        "ListBox", "Table", "ProgressBar", "Chart"
    }), "Dados");

    // 4. Containers
    toolBoxWidget->addItem(createCategory({
        "GroupBox", "Frame", "TabWidget"
    }), "Containers");

    dockToolbox->setWidget(toolBoxWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockToolbox);
    
    tabifyDockWidget(dockInspector, dockToolbox);

    // Right Dock: PROPERTIES
    QDockWidget *dockProps = new QDockWidget("Propriedades", this);
    m_propEditor = new PropertyEditor(dockProps);
    dockProps->setWidget(m_propEditor);
    addDockWidget(Qt::RightDockWidgetArea, dockProps);
}

void MainWindow::createSampleWidgets()
{
    // Teste de criação via Factory - Ainda útil para ver algo na tela ao iniciar
    QWidget *w1 = m_factory->createWidget("Label", "lbl_welcome");
    if (w1) {
        w1->setProperty("text", "<h1>Bem-vindo ao Showbox Studio</h1>");
        m_canvas->addWidget(w1);
        m_controller->manageWidget(w1);
    }
}

void MainWindow::onRunClicked()
{
    ScriptGenerator gen;
    QString script = gen.generate(m_canvas);
    
    if (script.isEmpty()) {
        statusBar()->showMessage("Erro: Nada para exportar.");
        return;
    }

    // Salvar temporário
    QString tempPath = "/tmp/studio_preview.sbx";
    QFile file(tempPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << script;
        file.close();
    } else {
        statusBar()->showMessage("Erro ao salvar arquivo temporário.");
        return;
    }

    statusBar()->showMessage("Executando Preview...");

    // Executar Showbox
    // Usamos o caminho relativo baseado na estrutura do workspace
    QString binPath = "../showbox/build/bin/poc_modern_cli";
    
    // QProcess para rodar o binário lendo o script
    QProcess *process = new QProcess(this);
    process->setProgram(binPath);
    // Passar argumento para manter aberto e input via arquivo (stdin)
    process->setArguments({"--keep-open"});
    process->setStandardInputFile(tempPath);

    connect(process, &QProcess::finished, [this, process](int exitCode) {
        if (exitCode == 0) {
            statusBar()->showMessage("Preview finalizado com sucesso.");
        } else {
            statusBar()->showMessage(QString("Erro no Preview (Exit code: %1)").arg(exitCode));
            qDebug() << "Showbox Error Output:" << process->readAllStandardError();
        }
        process->deleteLater();
    });

    process->start();
}

void MainWindow::onSaveClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Project", "", "Showbox Project (*.sbxproj)");
    if (fileName.isEmpty()) return;

    if (!fileName.endsWith(".sbxproj")) fileName += ".sbxproj";

    ProjectSerializer serializer;
    if (serializer.save(fileName, m_canvas)) {
        statusBar()->showMessage("Projeto salvo com sucesso: " + fileName);
    } else {
        statusBar()->showMessage("Erro ao salvar projeto.");
    }
}

void MainWindow::onOpenClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Project", "", "Showbox Project (*.sbxproj)");
    if (fileName.isEmpty()) return;

    m_canvas->clear();
    m_inspector->updateHierarchy(nullptr); 

    ProjectSerializer serializer;
    QList<QWidget*> widgets;
    
    if (serializer.load(fileName, m_factory, widgets)) {
        for (QWidget *w : widgets) {
            m_canvas->addWidget(w);
            m_controller->manageWidget(w);
        }
        m_inspector->updateHierarchy(m_canvas);
        statusBar()->showMessage("Projeto carregado: " + fileName);
    } else {
        statusBar()->showMessage("Erro ao carregar projeto.");
    }
}

void MainWindow::onDeleteClicked()
{
    QWidget *selected = m_controller->selectedWidget();
    if (!selected) return;

    // Não permitir deletar o Canvas em si ou algo vital
    if (selected == m_canvas) return;

    m_controller->undoStack()->push(new DeleteWidgetCommand(m_canvas, selected));
    
    m_controller->selectWidget(nullptr); 
    m_inspector->updateHierarchy(m_canvas); 
    statusBar()->showMessage("Componente removido (Undo disponível).");
}
