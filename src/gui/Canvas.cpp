#include "Canvas.h"
#include "../core/IStudioWidgetFactory.h"
#include "DesignerEventFilter.h"
#include <QMimeData>
#include <QDebug>
#include <QListWidget>

Canvas::Canvas(IStudioWidgetFactory *factory, QWidget *parent) 
    : QWidget(parent), m_factory(factory)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setAlignment(Qt::AlignTop);
    
    // Visual do Canvas (parecido com uma janela de diálogo)
    setStyleSheet("Canvas { background-color: #f0f0f0; border: 1px solid #ccc; }");
    
    setAcceptDrops(true);

    m_selectionFilter = new DesignerEventFilter(this);
    connect(m_selectionFilter, &DesignerEventFilter::widgetClicked, this, [this](QWidget *w) {
        emit widgetSelected(w);
        setSelectedWidget(w);
    });
}

void Canvas::addWidget(QWidget *widget)
{
    m_layout->addWidget(widget);
    widget->installEventFilter(m_selectionFilter);
}

void Canvas::setSelectedWidget(QWidget *widget)
{
    if (m_currentSelection) {
        // Restaurar estilo anterior (simplificado: remove borda azul)
        m_currentSelection->setStyleSheet(""); 
    }
    
    m_currentSelection = widget;
    
    if (m_currentSelection) {
        // Aplicar highlight
        m_currentSelection->setStyleSheet("outline: 2px dashed blue; border: 1px solid blue;");
    }
}

void Canvas::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText() || qobject_cast<QListWidget*>(event->source())) {
        event->acceptProposedAction();
    }
}

void Canvas::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void Canvas::dropEvent(QDropEvent *event)
{
    QString type;
    const QMimeData *mime = event->mimeData();
    
    if (mime->hasText()) {
        type = mime->text();
    } else if (QListWidget *list = qobject_cast<QListWidget*>(event->source())) {
        QList<QListWidgetItem*> items = list->selectedItems();
        if (!items.isEmpty()) {
            type = items.first()->text();
        }
    }

    if (type.isEmpty()) return;

    // Gerar um nome único simples
    QString name = QString("%1_%2").arg(type.toLower()).arg(m_layout->count() + 1);

    QWidget *newWidget = m_factory->createWidget(type, name);

    if (newWidget) {
        addWidget(newWidget);
        emit widgetAdded(newWidget);
        event->acceptProposedAction();
    }
}
