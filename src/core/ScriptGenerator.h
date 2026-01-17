#ifndef SCRIPTGENERATOR_H
#define SCRIPTGENERATOR_H

#include <QString>
#include <QWidget>
#include <QObject>

class ScriptGenerator
{
public:
    explicit ScriptGenerator();

    /**
     * @brief Generates a Showbox CLI script from the given widget hierarchy.
     * @param root The root widget (usually the content of the Canvas, or the Window).
     * @return A QString containing the full shell script with Here-Doc.
     */
    QString generate(QWidget *root);

private:
    /**
     * @brief Recursively processes a widget and its children.
     * @param widget The current widget to process.
     * @param indentLevel Indentation level for readability (optional in final script but good for debug).
     */
    void processWidget(QWidget *widget, QString &script);

    /**
     * @brief Maps internal widget types/class names to Showbox CLI command types.
     * @param widget The widget to identify.
     * @return String representing the type (e.g., "label", "button").
     */
    QString getShowboxType(QWidget *widget);

    /**
     * @brief Extracts relevant properties for the 'add' command.
     */
    QString getPropertiesString(QWidget *widget);
};

#endif // SCRIPTGENERATOR_H
