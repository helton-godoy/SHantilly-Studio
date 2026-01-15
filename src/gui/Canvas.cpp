#include "Canvas.h"
#include "../core/WidgetFactory.h"
#include <QMimeData>
#include <QDebug>

Canvas::Canvas(WidgetFactory *factory, QWidget *parent) 
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
    m_layout->addWidget(widget);
}

void Canvas::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    }
}

void Canvas::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void Canvas::dropEvent(QDropEvent *event)
{
    const QMimeData *mime = event->mimeData();
    if (!mime->hasText()) return;

    QString type = mime->text();
    QWidget *newWidget = nullptr;
    
    // Gerar um nome único simples
    QString name = QString("%1_%2").arg(type.toLower()).arg(m_layout->count() + 1);

    if (type == "PushButton") {
        newWidget = m_factory->createPushButton("Novo Botão", name);
    } else if (type == "Label") {
        newWidget = m_factory->createLabel("Texto", name);
    } else if (type == "Chart") {
        newWidget = m_factory->createChart("Gráfico", name);
    }

    if (newWidget) {
        addWidget(newWidget);
        emit widgetAdded(newWidget);
        event->acceptProposedAction();
        
        // Notificar o mundo (via parent/controller) que algo mudou seria ideal aqui,
        // mas o StudioController pega via EventFilter depois.
    }
}
