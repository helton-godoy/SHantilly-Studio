#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>

class WidgetFactory;

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(WidgetFactory *factory, QWidget *parent = nullptr);
    
    void addWidget(QWidget *widget);

signals:
    void widgetAdded(QWidget *widget);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QVBoxLayout *m_layout;
    WidgetFactory *m_factory;
};

#endif // CANVAS_H
