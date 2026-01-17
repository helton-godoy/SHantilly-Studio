#include "PropertyEditor.h"
#include "../core/StudioCommands.h"
#include "../core/StudioController.h"
#include <QHeaderView>
#include <QMetaProperty>
#include <QDebug>
#include <QComboBox>

PropertyEditor::PropertyEditor(QWidget *parent) : QTableWidget(parent)
{
    setColumnCount(2);
    setHorizontalHeaderLabels({"Propriedade", "Valor"});
    horizontalHeader()->setStretchLastSection(true);
    setAlternatingRowColors(true);
    
    connect(this, &QTableWidget::cellChanged, this, &PropertyEditor::onCellValueChanged);
}

void PropertyEditor::setTargetWidget(QWidget *widget)
{
    m_target = widget;
    m_isLoading = true;
    
    clearContents();
    setRowCount(0);

    if (!widget) {
        m_isLoading = false;
        return;
    }

    const QMetaObject *meta = widget->metaObject();
    
    // Iterar sobre todas as propriedades (incluindo herdadas)
    for (int i = 0; i < meta->propertyCount(); ++i) {
        QMetaProperty prop = meta->property(i);
        
        // Ignorar propriedades não editáveis ou internas
        if (!prop.isWritable() || !prop.isDesignable()) continue;
        
        // Vamos focar nas propriedades mais úteis por enquanto
        QVariant value = prop.read(widget);
        
        addPropertyRow(prop, value);
    }
    
    m_isLoading = false;
}

void PropertyEditor::addPropertyRow(const QMetaProperty &prop, const QVariant &value)
{
    int row = rowCount();
    insertRow(row);
    QString name = prop.name();
    
    // Nome da Propriedade (Read-only)
    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable);
    setItem(row, 0, nameItem);
    
    // Item base para a coluna de valor
    QTableWidgetItem *valueItem = new QTableWidgetItem();
    valueItem->setData(Qt::UserRole, name);
    
    if (prop.isEnumType()) {
        QComboBox *combo = new QComboBox();
        QMetaEnum metaEnum = prop.enumerator();
        for (int i = 0; i < metaEnum.keyCount(); ++i) {
            combo->addItem(metaEnum.key(i), metaEnum.value(i));
        }
        
        // Selecionar valor atual
        // Para flags ou enums combinados, isso pode ser complexo, mas para enums simples funciona
        int index = combo->findData(value.toInt());
        if (index >= 0) combo->setCurrentIndex(index);
        
        // Conectar sinal
        connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
                this, [this, name, combo](int) {
            if (m_isLoading || !m_target) return;
            int val = combo->currentData().toInt();
            QVariant oldVal = m_target->property(name.toUtf8().constData());
            
            if (m_controller) {
                m_controller->undoStack()->push(new PropertyChangeCommand(m_target, name, oldVal, val));
            } else {
                m_target->setProperty(name.toUtf8().constData(), val);
            }
        });

        setCellWidget(row, 1, combo);
        setItem(row, 1, valueItem); // Dummy item por trás
        
    } else if (value.typeId() == QMetaType::Bool) {
        valueItem->setCheckState(value.toBool() ? Qt::Checked : Qt::Unchecked);
        valueItem->setText(value.toBool() ? "true" : "false");
        setItem(row, 1, valueItem);
    } else {
        valueItem->setText(value.toString());
        setItem(row, 1, valueItem);
    }
}

void PropertyEditor::onCellValueChanged(int row, int col)
{
    // Se estivermos carregando a tabela, ignorar (evita loops)
    if (m_isLoading || !m_target || col != 1) return;
    
    QTableWidgetItem *valueItem = item(row, 1);
    QString propName = valueItem->data(Qt::UserRole).toString();
    
    // Ler o tipo da propriedade original para converter corretamente
    QVariant oldValue = m_target->property(propName.toUtf8().constData());
    QVariant newValue;

    if (oldValue.typeId() == QMetaType::Bool) {
        // Se o usuário mudou o texto ou o check state
        bool b = (valueItem->text().toLower() == "true" || valueItem->checkState() == Qt::Checked);
        newValue = b;
        
        // Sincronizar texto e checkstate visualmente
        m_isLoading = true;
        valueItem->setCheckState(b ? Qt::Checked : Qt::Unchecked);
        valueItem->setText(b ? "true" : "false");
        m_isLoading = false;
    } else {
        newValue = valueItem->text();
    }
    
    // Aplicar o novo valor ao widget via Command
    if (!propName.isEmpty()) {
        if (m_controller) {
            m_controller->undoStack()->push(new PropertyChangeCommand(m_target, propName, oldValue, newValue));
        } else {
            m_target->setProperty(propName.toUtf8().constData(), newValue);
        }
    }
}
