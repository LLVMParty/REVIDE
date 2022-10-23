#pragma once

#include <QDialog>

#include "widgets/SimpleTextGraphView.h"

class TestGraphView : public SimpleTextGraphView
{
    Q_OBJECT

public:
    explicit TestGraphView(QWidget *parent);

protected:
    void loadCurrentGraph() override;
};

namespace Ui
{
class GraphDialog;
}

class GraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphDialog(QWidget* parent = nullptr);
    ~GraphDialog();
    void setHtml(const QString& html);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::GraphDialog* ui = nullptr;
    SimpleTextGraphView* mGraphView = nullptr;
};
