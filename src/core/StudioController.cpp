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
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QWidget *widget = qobject_cast<QWidget*>(watched);
        if (widget) {
            // Se for clique com botão direito em um item já selecionado,
            // não alteramos a seleção para permitir que o menu de contexto
            // atue sobre o grupo atual.
            if (mouseEvent->button() == Qt::RightButton && m_selectedWidgets.contains(widget)) {
                return false; // Deixa o evento propagar para disparar o ContextMenu
            }

            if (mouseEvent->modifiers() & Qt::ControlModifier) {
                multiSelectWidget(widget);
            } else {
                selectWidget(widget);
            }
            return true; 
        }
    }
    return QObject::eventFilter(watched, event);
}

void StudioController::selectWidget(QWidget *widget)
{
    // Limpar destaques antigos
    for (QWidget *w : m_selectedWidgets) {
        if (w) w->setStyleSheet("");
    }
    m_selectedWidgets.clear();

    if (widget) {
        m_selectedWidgets.append(widget);
        widget->setStyleSheet("border: 2px solid #0078d7;");
    }

    emit selectionChanged();
    emit widgetSelected(widget);
}

void StudioController::multiSelectWidget(QWidget *widget)
{
    if (!widget) return;

    if (m_selectedWidgets.contains(widget)) {
        widget->setStyleSheet("");
        m_selectedWidgets.removeOne(widget);
    } else {
        m_selectedWidgets.append(widget);
        widget->setStyleSheet("border: 2px solid #0078d7;");
    }

    emit selectionChanged();
    emit widgetSelected(m_selectedWidgets.isEmpty() ? nullptr : m_selectedWidgets.last());
}
