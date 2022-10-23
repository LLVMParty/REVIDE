#include "GraphDialog.h"
#include "ui_GraphDialog.h"
#include "FunctionListModel.h"
#include "QtHelpers.h"

#include <QStringListModel>

#include <QMessageBox>

#include "widgets/SimpleTextGraphView.h"

TestGraphView::TestGraphView(QWidget* parent)
    : SimpleTextGraphView(parent, nullptr /* fake MainWindow */)
{
}

void TestGraphView::loadCurrentGraph()
{
    qDebug() << "loadCurrentGraph()";

    blockContent.clear();
    blocks.clear();

    std::unordered_set<ut64> edges;
    auto addBasicBlock = [&](RVA address, const QString& name, const std::unordered_set<ut64>& destinations) {
        GraphLayout::GraphBlock block;
        block.entry = address;

        for(const auto& x : destinations) {
            block.edges.emplace_back(x);
            edges.insert(x);
        }

        addBlock(std::move(block), name, address);
    };

    addBasicBlock(0x1000, "BB0", { 0x1001 });
    addBasicBlock(0x1001, "BB1", {0x1002, 0x1003 });
    addBasicBlock(0x1002, "BB2", {0x1004});
    addBasicBlock(0x1003, "BB3", {0x1004});
    addBasicBlock(0x1004, "BB4", {});

    // Only for unknown destinations
    for(const auto& x : edges) {
        if(blockContent.find(x) != blockContent.end()) {
            // Already visited
            continue;
        }

        GraphLayout::GraphBlock block;
        block.entry = x;
        auto name = QString("BB_UNK_%1").arg(RzAddressString(x));
        this->addBlock(block, name, x);
    }

    computeGraphPlacement();
}

GraphDialog::GraphDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::GraphDialog)
{
    setWindowFlags(Qt::Tool);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->setupUi(this);
    qtRestoreGeometry(this);

    mGraphView = new TestGraphView(this);
    ui->verticalLayout->addWidget(mGraphView);
    // TODO: pass some kind of graph
    mGraphView->refreshView();
}

GraphDialog::~GraphDialog()
{
    delete ui;
}

void GraphDialog::setHtml(const QString& html)
{
}

void GraphDialog::closeEvent(QCloseEvent* event)
{
    qtSaveGeometry(this);
    QDialog::closeEvent(event);
}

