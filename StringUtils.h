#pragma once

#include <QString>
#include <QtGlobal>

class StringUtils
{
public:
    StringUtils() = delete;
    
    static QString toCamelCase(const QString &input);
    static QString toSnakeCase(const QString &input);
};