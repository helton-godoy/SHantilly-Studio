#ifndef STUDIOCONTROLLER_H
#define STUDIOCONTROLLER_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QUndoStack>

class StudioController : public QObject
{
    Q_OBJECT
public:
    explicit StudioController(QObject *parent = nullptr);

    // Adiciona um widget ao controle do editor
    void manageWidget(QWidget *widget);
    void selectWidget(QWidget *widget);

    QWidget* selectedWidget() const { return m_selectedWidget; }
    QUndoStack* undoStack() const { return m_undoStack; }

signals:
    void widgetSelected(QWidget *widget);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QWidget *m_selectedWidget = nullptr;
    QUndoStack *m_undoStack = nullptr;
};

#endif // STUDIOCONTROLLER_H
