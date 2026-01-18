#include <QTest>
#include <QCoreApplication>
#include "StringUtils.h"

class StringUtilsTest : public QObject
{
    Q_OBJECT

private slots:
    void testToCamelCase();
    void testToCamelCase_data();
    void testToSnakeCase();
    void testToSnakeCase_data();
    void testEdgeCases();
};

void StringUtilsTest::testToCamelCase_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expected");

    QTest::newRow("simple snake") << "hello_world" << "helloWorld";
    QTest::newRow("multiple underscores") << "this_is_a_test" << "thisIsATest";
    QTest::newRow("single word") << "hello" << "hello";
    QTest::newRow("already camel") << "helloWorld" << "helloWorld";
    QTest::newRow("hyphenated") << "hello-world" << "helloWorld";
    QTest::newRow("mixed separators") << "hello_world-test" << "helloWorldTest";
    QTest::newRow("uppercase input") << "HELLO_WORLD" << "helloWorld";
    QTest::newRow("numbers") << "hello_world_123" << "helloWorld123";
    QTest::newRow("leading underscore") << "_hello_world" << "helloWorld";
    QTest::newRow("trailing underscore") << "hello_world_" << "helloWorld";
    QTest::newRow("multiple consecutive underscores") << "hello__world" << "helloWorld";
}

void StringUtilsTest::testToCamelCase()
{
    QFETCH(QString, input);
    QFETCH(QString, expected);

    QString result = StringUtils::toCamelCase(input);
    QCOMPARE(result, expected);
}

void StringUtilsTest::testToSnakeCase_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expected");

    QTest::newRow("simple camel") << "helloWorld" << "hello_world";
    QTest::newRow("multiple words") << "thisIsATest" << "this_is_a_test";
    QTest::newRow("single word") << "hello" << "hello";
    QTest::newRow("already snake") << "hello_world" << "hello_world";
    QTest::newRow("pascal case") << "HelloWorld" << "hello_world";
    QTest::newRow("acronym") << "XMLParser" << "xml_parser";
    QTest::newRow("mixed case") << "HelloWorldTest" << "hello_world_test";
    QTest::newRow("numbers") << "helloWorld123" << "hello_world123";
    QTest::newRow("spaces") << "hello world" << "hello_world";
    QTest::newRow("hyphens") << "hello-world" << "hello_world";
    QTest::newRow("mixed separators") << "hello world-test" << "hello_world_test";
}

void StringUtilsTest::testToSnakeCase()
{
    QFETCH(QString, input);
    QFETCH(QString, expected);

    QString result = StringUtils::toSnakeCase(input);
    QCOMPARE(result, expected);
}

void StringUtilsTest::testEdgeCases()
{
    QCOMPARE(StringUtils::toCamelCase(""), "");
    QCOMPARE(StringUtils::toSnakeCase(""), "");
    
    QCOMPARE(StringUtils::toCamelCase("_"), "");
    QCOMPARE(StringUtils::toSnakeCase("_"), "");
    
    QCOMPARE(StringUtils::toCamelCase("__"), "");
    QCOMPARE(StringUtils::toSnakeCase("__"), "");
    
    QCOMPARE(StringUtils::toCamelCase("a"), "a");
    QCOMPARE(StringUtils::toSnakeCase("a"), "a");
    
    QCOMPARE(StringUtils::toCamelCase("A"), "a");
    QCOMPARE(StringUtils::toSnakeCase("A"), "a");
}

QTEST_MAIN(StringUtilsTest)
#include "main.moc"