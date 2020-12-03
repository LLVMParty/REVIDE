#pragma once

#include "AbstractFunctionList.h"

#include <QAbstractListModel>

class FunctionListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    FunctionListModel(const AbstractFunctionList* functionList, QObject* parent = 0)
        : QAbstractListModel(parent)
        , mFunctionList(functionList) {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override
    {
        return 1;
        //return mFunctionList.size();
    }

    QVariant data(const QModelIndex& index, int role) const override
    {
        return "chuj";
        //return mFunctionList[index.row()];
    }

    Qt::ItemFlags flags(const QModelIndex& index)
    {
        return Qt::ItemIsEnabled;
    }

private:
    const AbstractFunctionList* mFunctionList = nullptr;
};