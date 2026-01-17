#include <QtTest>
#include "core/ScriptGenerator.h"
#include "core/StudioWidgetFactory.h"
#include "gui/Canvas.h"

class tst_ScriptGenerator : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testBasicGeneration();
    void testProperties();
    void testContainer();

private:
    StudioWidgetFactory *m_factory;
    QWidget *m_root;
};

void tst_ScriptGenerator::initTestCase()
{
    m_factory = new StudioWidgetFactory();
    // Usamos um widget simples como root container
    m_root = new QWidget();
}

void tst_ScriptGenerator::cleanupTestCase()
{
    delete m_root;
    delete m_factory;
}

void tst_ScriptGenerator::testBasicGeneration()
{
    // Limpar root
    qDeleteAll(m_root->children());

    QWidget *btn = m_factory->createWidget("PushButton", "btn1");
    btn->setParent(m_root);

    QWidget *lbl = m_factory->createWidget("Label", "lbl1");
    lbl->setParent(m_root);

    ScriptGenerator gen;
    QString output = gen.generate(m_root);

    qDebug() << "Generated Script:\n" << output;

    QVERIFY(output.contains("#!/bin/bash"));
    QVERIFY(output.contains("showbox << EOD"));
    // Verificar linhas específicas
    QVERIFY(output.contains("add pushbutton \"Button\" btn1"));
    QVERIFY(output.contains("add label \"Label\" lbl1"));
    QVERIFY(output.contains("EOD"));
}

void tst_ScriptGenerator::testProperties()
{
    qDeleteAll(m_root->children());

    QWidget *chk = m_factory->createWidget("CheckBox", "chk1");
    chk->setProperty("text", "Remember Me");
    chk->setProperty("checked", true);
    chk->setParent(m_root);

    ScriptGenerator gen;
    QString output = gen.generate(m_root);

    qDebug() << "Properties Script:\n" << output;

    QVERIFY(output.contains("add checkbox \"Remember Me\" chk1"));
    QVERIFY(output.contains("checked"));
}

void tst_ScriptGenerator::testContainer()
{
    qDeleteAll(m_root->children());

    QWidget *group = m_factory->createWidget("GroupBox", "grp1");
    group->setProperty("title", "Options");
    group->setParent(m_root);

    // Adicionar filho ao grupo
    // Nota: Como o Studio gerencia o parentesco, simulamos aqui
    QWidget *radio = m_factory->createWidget("RadioButton", "rad1");
    
    // O layout do GroupBox deve receber o widget
    if (group->layout()) {
        group->layout()->addWidget(radio);
    } else {
        radio->setParent(group);
    }

    ScriptGenerator gen;
    QString output = gen.generate(m_root);
    
    qDebug() << "Container Script:\n" << output;

    // A ordem importa para o parser se não houver contexto explícito, 
    // mas o gerador recursivo deve processar o pai antes dos filhos.
    
    int idxGrp = output.indexOf("add groupbox \"Options\" grp1");
    int idxRad = output.indexOf("add radiobutton \"RadioButton\" rad1");
    
    QVERIFY(idxGrp != -1);
    QVERIFY(idxRad != -1);
    QVERIFY(idxGrp < idxRad); // Pai antes do filho
}

QTEST_MAIN(tst_ScriptGenerator)
#include "tst_ScriptGenerator.moc"
