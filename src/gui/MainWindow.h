#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Canvas;
class IStudioWidgetFactory;
class ObjectInspector;
class PropertyEditor;
class StudioController;
class ScriptGenerator;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRunClicked();
    void onSaveClicked();
    void onOpenClicked();
    void onDeleteClicked();

private:
    void setupUI();
    void createSampleWidgets();

    Canvas *m_canvas;
    IStudioWidgetFactory *m_factory;
    ObjectInspector *m_inspector;
    StudioController *m_controller;
    PropertyEditor *m_propEditor;
};

#endif // MAINWINDOW_H
