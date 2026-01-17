#ifndef STUDIOCONTROLLER_H
#define STUDIOCONTROLLER_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>

class StudioController : public QObject
{
    Q_OBJECT
public:
    explicit StudioController(QObject *parent = nullptr);

    // Adiciona um widget ao controle do editor
    void manageWidget(QWidget *widget);
    void selectWidget(QWidget *widget);

signals:
    void widgetSelected(QWidget *widget);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QWidget *m_selectedWidget = nullptr;
};

#endif // STUDIOCONTROLLER_H
