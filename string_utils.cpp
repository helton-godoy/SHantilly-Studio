#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include <QtTest/QtTest>

class StringUtils {
public:
    static QString toCamelCase(const QString&amp; input) {
        if (input.isEmpty()) return input;
        QStringList parts = input.split('_', Qt::SkipEmptyParts);
        if (parts.isEmpty()) return input;
        for (int i = 1; i &lt; parts.size(); ++i) {
            if (!parts[i].isEmpty()) {
                parts[i] = parts[i].left(1).toUpper() + parts[i].mid(1).toLower();
            }
        }
        return parts.join("");
    }

    static QString toSnakeCase(const QString&amp; input) {
        if (input.isEmpty()) return input;
        QString result;
        for (QChar c : input) {
            if (c.isUpper()) {
                if (!result.isEmpty()) {
                    result += '_';
                }
                result += c.toLower();
            } else {
                result += c;
            }
        }
        return result;
    }
};

class TestStringUtils : public QObject {
    Q_OBJECT

private slots:
    void testToCamelCase() {
        QCOMPARE(StringUtils::toCamelCase("hello_world"), QString("helloWorld"));
        QCOMPARE(StringUtils::toCamelCase("foo_bar_baz"), QString("fooBarBaz"));
        QCOMPARE(StringUtils::toCamelCase("single"), QString("single"));
        QCOMPARE(StringUtils::toCamelCase(""), QString(""));
        QCOMPARE(StringUtils::toCamelCase("a_b_c_d"), QString("aBCD"));
    }

    void testToSnakeCase() {
        QCOMPARE(StringUtils::toSnakeCase("helloWorld"), QString("hello_world"));
        QCOMPARE(StringUtils::toSnakeCase("fooBarBaz"), QString("foo_bar_baz"));
        QCOMPARE(StringUtils::toSnakeCase("single"), QString("single"));
        QCOMPARE(StringUtils::toSnakeCase(""), QString(""));
        QCOMPARE(StringUtils::toSnakeCase("ABC"), QString("a_b_c"));
    }
};

#include "string_utils.moc"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    TestStringUtils test;
    return QTest::qExec(&amp;test, argc, argv);
}