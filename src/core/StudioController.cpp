#include "StudioController.h"
#include <QFrame>

StudioController::StudioController(QObject *parent) : QObject(parent)
{
    m_undoStack = new QUndoStack(this);
}

void StudioController::manageWidget(QWidget *widget)
{
    // Instalamos o filtro para interceptar cliques
    widget->installEventFilter(this);
    
    // Recursivamente para filhos (ex: botões dentro de frames)
    for (QObject *child : widget->children()) {
        if (QWidget *w = qobject_cast<QWidget*>(child)) {
            manageWidget(w);
        }
    }
}

bool StudioController::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QWidget *widget = qobject_cast<QWidget*>(watched);
        if (widget) {
            selectWidget(widget);
            // Retornamos true para "comer" o evento e impedir que o widget
            // processe o clique normalmente (ex: botão não afunda)
            return true; 
        }
    }
    return QObject::eventFilter(watched, event);
}

void StudioController::selectWidget(QWidget *widget)
{
    if (m_selectedWidget == widget) return;

    // Remover destaque do antigo
    if (m_selectedWidget) {
        m_selectedWidget->setStyleSheet("");
    }

    m_selectedWidget = widget;

    // Aplicar destaque visual (borda azul)
    // Nota: Em um editor real usaríamos um Overlay ou RubberBand, 
    // mas stylesheet é rápido para o protótipo.
    if (m_selectedWidget) {
        m_selectedWidget->setStyleSheet("border: 2px solid #0078d7;");
        emit widgetSelected(m_selectedWidget);
    }
}
