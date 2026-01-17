#ifndef DESIGNEREVENTFILTER_H
#define DESIGNEREVENTFILTER_H

#include <QObject>
#include <QWidget>
#include <QEvent>

class DesignerEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit DesignerEventFilter(QObject *parent = nullptr);

signals:
    void widgetClicked(QWidget *widget);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // DESIGNEREVENTFILTER_H