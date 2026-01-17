#ifndef STUDIOCOMMANDS_H
#define STUDIOCOMMANDS_H

#include <QUndoCommand>
#include <QWidget>
#include "gui/Canvas.h"

class AddWidgetCommand : public QUndoCommand
{
public:
    AddWidgetCommand(Canvas *canvas, QWidget *widget, QUndoCommand *parent = nullptr)
        : QUndoCommand(parent), m_canvas(canvas), m_widget(widget)
    {
        setText("Add " + widget->objectName());
    }

    void undo() override {
        m_canvas->removeWidget(m_widget);
    }

    void redo() override {
        m_canvas->addWidget(m_widget);
    }

private:
    Canvas *m_canvas;
    QWidget *m_widget;
};

class DeleteWidgetCommand : public QUndoCommand
{
public:
    DeleteWidgetCommand(Canvas *canvas, QWidget *widget, QUndoCommand *parent = nullptr)
        : QUndoCommand(parent), m_canvas(canvas), m_widget(widget)
    {
        setText("Delete " + widget->objectName());
    }

    void undo() override {
        m_canvas->addWidget(m_widget);
    }

    void redo() override {
        m_canvas->removeWidget(m_widget);
    }

private:
    Canvas *m_canvas;
    QWidget *m_widget;
};

class PropertyChangeCommand : public QUndoCommand
{
public:
    PropertyChangeCommand(QWidget *target, const QString &propertyName, const QVariant &oldValue, const QVariant &newValue, QUndoCommand *parent = nullptr)
        : QUndoCommand(parent), m_target(target), m_propertyName(propertyName), m_oldValue(oldValue), m_newValue(newValue)
    {
        setText(QString("Change %1 of %2").arg(propertyName, target->objectName()));
    }

    void undo() override {
        m_target->setProperty(m_propertyName.toUtf8().constData(), m_oldValue);
    }

    void redo() override {
        m_target->setProperty(m_propertyName.toUtf8().constData(), m_newValue);
    }

private:
    QWidget *m_target;
    QString m_propertyName;
    QVariant m_oldValue;
    QVariant m_newValue;
};

#endif // STUDIOCOMMANDS_H
