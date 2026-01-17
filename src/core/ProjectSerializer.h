#ifndef PROJECTSERIALIZER_H
#define PROJECTSERIALIZER_H

#include <QString>
#include <QWidget>
#include <QJsonObject>

class IStudioWidgetFactory;

class ProjectSerializer
{
public:
    explicit ProjectSerializer();

    /**
     * @brief Saves the widget hierarchy to a JSON file.
     * @param filename Full path to the output file.
     * @param root The root widget (usually the Canvas content container).
     * @return true if successful.
     */
    bool save(const QString &filename, QWidget *root);

    /**
     * @brief Loads a project from a JSON file and reconstructs the widgets.
     * @param filename Full path to the input file.
     * @param factory Factory to create widgets.
     * @param outWidgets List to populate with created root widgets (caller takes ownership).
     * @return true if successful.
     */
    bool load(const QString &filename, IStudioWidgetFactory *factory, QList<QWidget*> &outWidgets);

private:
    QJsonObject serializeWidget(QWidget *widget);
    QWidget* deserializeWidget(const QJsonObject &json, IStudioWidgetFactory *factory);
};

#endif // PROJECTSERIALIZER_H
