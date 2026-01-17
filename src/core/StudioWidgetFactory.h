#ifndef STUDIOWIDGETFACTORY_H
#define STUDIOWIDGETFACTORY_H

#include "IStudioWidgetFactory.h"
#include <ShowboxBuilder.h>
#include <WidgetConfigs.h>

class StudioWidgetFactory : public IStudioWidgetFactory {
public:
    QWidget* createWidget(const QString &type, const QString &name) override {
        ShowboxBuilder builder;
        QString t = type.toLower();
        QWidget *w = nullptr;

        if (t == "pushbutton" || t == "button") {
            Showbox::Models::ButtonConfig cfg;
            cfg.name = name;
            cfg.text = "Button";
            w = builder.buildButton(cfg);
        } else if (t == "label") {
            Showbox::Models::LabelConfig cfg;
            cfg.name = name;
            cfg.text = "Label";
            w = builder.buildLabel(cfg);
        } else if (t == "checkbox") {
            Showbox::Models::CheckBoxConfig cfg;
            cfg.name = name;
            cfg.text = "CheckBox";
            w = builder.buildCheckBox(cfg);
        } else if (t == "radiobutton") {
            Showbox::Models::RadioButtonConfig cfg;
            cfg.name = name;
            cfg.text = "RadioButton";
            w = builder.buildRadioButton(cfg);
        } else if (t == "spinbox") {
            Showbox::Models::SpinBoxConfig cfg;
            cfg.name = name;
            w = builder.buildSpinBox(cfg);
        } else if (t == "slider") {
            Showbox::Models::SliderConfig cfg;
            cfg.name = name;
            w = builder.buildSlider(cfg);
        } else if (t == "lineedit" || t == "textbox") {
            Showbox::Models::LineEditConfig cfg;
            cfg.name = name;
            cfg.placeholder = "Type here...";
            w = builder.buildLineEdit(cfg);
        } else if (t == "textedit" || t == "textview") {
            Showbox::Models::TextEditConfig cfg;
            cfg.name = name;
            cfg.text = "Edit me";
            w = builder.buildTextEdit(cfg);
        } else if (t == "groupbox") {
            Showbox::Models::GroupBoxConfig cfg;
            cfg.name = name;
            cfg.title = "Group";
            w = builder.buildGroupBox(cfg);
        } else if (t == "frame") {
            Showbox::Models::FrameConfig cfg;
            cfg.name = name;
            w = builder.buildFrame(cfg);
        } else if (t == "tabwidget" || t == "tabs") {
            Showbox::Models::TabWidgetConfig cfg;
            cfg.name = name;
            w = builder.buildTabWidget(cfg);
        } else if (t == "combobox") {
            Showbox::Models::ComboBoxConfig cfg;
            cfg.name = name;
            w = builder.buildComboBox(cfg);
        } else if (t == "listbox") {
            Showbox::Models::ListConfig cfg;
            cfg.name = name;
            w = builder.buildList(cfg);
        } else if (t == "calendar") {
            Showbox::Models::CalendarConfig cfg;
            cfg.name = name;
            w = builder.buildCalendar(cfg);
        } else if (t == "progressbar") {
            Showbox::Models::ProgressBarConfig cfg;
            cfg.name = name;
            w = builder.buildProgressBar(cfg);
        } else if (t == "chart") {
            Showbox::Models::ChartConfig cfg;
            cfg.name = name;
            cfg.title = "Chart";
            w = builder.buildChart(cfg);
        } else if (t == "separator") {
            Showbox::Models::SeparatorConfig cfg;
            cfg.name = name;
            w = builder.buildSeparator(cfg);
        }

        if (w) {
            setupStudioWidget(w);
        }
        return w;
    }

private:
    void setupStudioWidget(QWidget *widget) {
        if (!widget) return;
        // Atributo para garantir que o widget aceite eventos mas possa ser "gerenciado" pelo Studio
        widget->setAttribute(Qt::WA_TransparentForMouseEvents, false); 
    }
};

#endif // STUDIOWIDGETFACTORY_H