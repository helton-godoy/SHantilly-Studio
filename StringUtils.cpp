#include "StringUtils.h"
#include <QRegularExpression>
#include <QStringList>

QString StringUtils::toCamelCase(const QString &input)
{
    if (input.isEmpty()) {
        return input;
    }
    
    QString result = input.toLower();
    result.replace('_', ' ');
    result.replace('-', ' ');
    result = result.simplified();
    
    if (result.isEmpty()) {
        return result;
    }
    
    QStringList words = result.split(' ', Qt::SkipEmptyParts);
    result = words[0];
    
    for (int i = 1; i < words.size(); ++i) {
        QString word = words[i];
        if (!word.isEmpty()) {
            word[0] = word[0].toUpper();
            result += word;
        }
    }
    
    return result;
}

QString StringUtils::toSnakeCase(const QString &input)
{
    if (input.isEmpty()) {
        return input;
    }
    
    QString result = input;
    
    QRegularExpression regex("([a-z])([A-Z])");
    result.replace(regex, "\\1_\\2");
    
    QRegularExpression upperRegex("([A-Z]+)([A-Z][a-z])");
    result.replace(upperRegex, "\\1_\\2");
    
    result = result.toLower();
    result.replace(' ', '_');
    result.replace('-', '_');
    result.replace(QRegularExpression("_+"), "_");
    result = result.trimmed();
    result.remove(QRegularExpression("^_|_$"));
    
    return result;
}