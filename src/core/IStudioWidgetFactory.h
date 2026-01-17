#ifndef ISTUDIOWIDGETFACTORY_H
#define ISTUDIOWIDGETFACTORY_H

#include <QWidget>
#include <QString>

class IStudioWidgetFactory {
public:
    virtual ~IStudioWidgetFactory() = default;
    
    virtual QWidget* createWidget(const QString &type, const QString &name) = 0;
};

#endif // ISTUDIOWIDGETFACTORY_H
