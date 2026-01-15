#include "MainWindow.h"
#include "Canvas.h"
#include "ObjectInspector.h"
#include "PropertyEditor.h"
#include "core/StudioWidgetFactory.h"
#include "core/StudioController.h"
#include <QStatusBar>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Showbox Studio");
    resize(1200, 800);
    
    m_factory = new StudioWidgetFactory();
    m_controller = new StudioController(this);
    
    setupUI();
    createSampleWidgets();
    
    // Sincronizar seleção: Canvas -> Inspector & Property Editor
    connect(m_controller, &StudioController::widgetSelected, m_inspector, &ObjectInspector::selectItemForWidget);
    connect(m_controller, &StudioController::widgetSelected, m_propEditor, &PropertyEditor::setTargetWidget);
    
    // Conectar novos widgets do Canvas ao Controller e atualizar árvore
    connect(m_canvas, &Canvas::widgetAdded, m_controller, &StudioController::manageWidget);
    connect(m_canvas, &Canvas::widgetAdded, [this]() {
        m_inspector->updateHierarchy(m_canvas);
    });
    
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
    QListWidget *listToolbox = new QListWidget(dockToolbox);
    
    // Configurar Drag & Drop na Toolbox
    listToolbox->setDragEnabled(true);
    listToolbox->setDragDropMode(QAbstractItemView::DragOnly);
    
    listToolbox->addItem("PushButton");
    listToolbox->addItem("Label");
    listToolbox->addItem("Chart");
    dockToolbox->setWidget(listToolbox);
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
    QWidget *w1 = m_factory->createLabel("<h1>Bem-vindo ao Showbox Studio</h1>", "lbl_welcome");
    m_canvas->addWidget(w1);
    m_controller->manageWidget(w1);
}


