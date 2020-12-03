#pragma once

#include <QString>

struct AbstractFunctionList
{
    virtual ~AbstractFunctionList() = default;
    virtual size_t size() const = 0;
    virtual const QString& operator[](size_t index) const = 0;
    virtual void clicked(size_t index) = 0;
};