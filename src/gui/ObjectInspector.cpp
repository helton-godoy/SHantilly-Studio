#include "ObjectInspector.h"
#include <QTabWidget>

ObjectInspector::ObjectInspector(QWidget *parent) : QTreeWidget(parent)
{
    setHeaderLabel("Hierarquia de Objetos");
    setColumnCount(1);
    
    connect(this, &QTreeWidget::currentItemChanged, this, &ObjectInspector::onCurrentItemChanged);
}

void ObjectInspector::updateHierarchy(QWidget *root)
{
    clear();
    m_widgetToItem.clear();
    
    if (!root) return;
    
    addWidgetToTree(root, invisibleRootItem());
    expandAll();
}

void ObjectInspector::addWidgetToTree(QWidget *widget, QTreeWidgetItem *parentItem)
{
    if (!widget) return;

    QString name = widget->objectName();
    if (name.isEmpty()) name = widget->metaObject()->className();

    QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
    item->setText(0, name);
    item->setData(0, Qt::UserRole, QVariant::fromValue(widget));
    
    m_widgetToItem[widget] = item;

    // Verificar se deve recursar: Apenas containers devem expor seus filhos na árvore.
    // Widgets atômicos (Button, ComboBox) têm filhos internos (Qt implementation details) que devem ser ocultados.
    QVariant typeVar = widget->property("showbox_type");
    bool isContainer = true; // Por padrão, assumimos container (útil para o Canvas/Root)

    if (typeVar.isValid()) {
        QString type = typeVar.toString();
        // Lista explícita de containers
        isContainer = (type == "window" || type == "groupbox" || type == "frame" || 
                       type == "tabwidget" || type == "page");
    }

    if (!isContainer) return;

    // Caso Especial: QTabWidget
    if (QTabWidget *tab = qobject_cast<QTabWidget*>(widget)) {
        for (int i = 0; i < tab->count(); ++i) {
            addWidgetToTree(tab->widget(i), item);
        }
    }
    // Caso Padrão: Iterar filhos diretos (filtrando internos do Qt)
    else {
        for (QObject *child : widget->children()) {
            if (QWidget *w = qobject_cast<QWidget*>(child)) {
                // Ignorar widgets internos do Qt (como layouts ou barras)
                if (!w->objectName().startsWith("qt_")) {
                    addWidgetToTree(w, item);
                }
            }
        }
    }
}

void ObjectInspector::selectItemForWidget(QWidget *widget)
{
    if (m_widgetToItem.contains(widget)) {
        blockSignals(true);
        setCurrentItem(m_widgetToItem[widget]);
        blockSignals(false);
    }
}

void ObjectInspector::onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if (current) {
        QWidget *widget = current->data(0, Qt::UserRole).value<QWidget*>();
        emit itemSelected(widget);
    }
}
