#include "Canvas.h"
#include "../core/IStudioWidgetFactory.h"
#include "../core/StudioCommands.h"
#include "../core/StudioController.h"
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
}

void Canvas::addWidget(QWidget *widget)
{
    if (!widget) return;
    widget->setParent(this);
    m_layout->addWidget(widget);
    widget->show();
}

void Canvas::removeWidget(QWidget *widget)
{
    if (!widget) return;
    m_layout->removeWidget(widget);
    widget->hide();
    widget->setParent(nullptr); // Desvincular para não aparecer no children() se o teste checar o pai
}

void Canvas::clear()
{
    setSelectedWidget(nullptr);
    QLayoutItem *item;
    while ((item = m_layout->takeAt(0)) != nullptr) {
        if (QWidget *w = item->widget()) {
            delete w;
        }
        delete item;
    }
}

void Canvas::setSelectedWidget(QWidget *widget)
{
    m_currentSelection = widget;
    // A lógica visual de seleção agora é tratada pelo StudioController
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
        if (m_controller) {
            m_controller->undoStack()->push(new AddWidgetCommand(this, newWidget));
        } else {
            addWidget(newWidget);
        }
        emit widgetAdded(newWidget);
        event->acceptProposedAction();
    }
}
