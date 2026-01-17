#include "DesignerEventFilter.h"
#include <QMouseEvent>

DesignerEventFilter::DesignerEventFilter(QObject *parent) 
    : QObject(parent) 
{
}

bool DesignerEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QWidget *widget = qobject_cast<QWidget*>(obj);
        if (widget) {
            emit widgetClicked(widget);
            return true; // Consumir o evento para não ativar o widget
        }
    }
    return QObject::eventFilter(obj, event);
}