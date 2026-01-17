#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>

class IStudioWidgetFactory;
class StudioController;

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(IStudioWidgetFactory *factory, QWidget *parent = nullptr);
    
    void setController(StudioController *controller) { m_controller = controller; }
    
    void addWidget(QWidget *widget);
    void removeWidget(QWidget *widget);
    void setSelectedWidget(QWidget *widget);
    void clear();

signals:
    void widgetAdded(QWidget *widget);
    void widgetSelected(QWidget *widget);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QVBoxLayout *m_layout;
    IStudioWidgetFactory *m_factory;
    StudioController *m_controller = nullptr;
    QWidget *m_currentSelection = nullptr;
};

#endif // CANVAS_H
