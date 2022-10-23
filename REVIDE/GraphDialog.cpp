#include "GraphDialog.h"
#include "ui_GraphDialog.h"
#include "FunctionListModel.h"
#include "QtHelpers.h"

#include <QTimer>
#include <QMessageBox>

#include "widgets/SimpleTextGraphView.h"

GenericGraphView::GenericGraphView(QWidget* parent)
    : SimpleTextGraphView(parent, nullptr /* fake MainWindow */)
{
}

void GenericGraphView::loadCurrentGraph()
{
    static int counter = 0;
    qDebug() << "loadCurrentGraph()" << counter++;

    blockContent.clear();
    blocks.clear();

    std::unordered_set<ut64> edges;
    for(const auto& node : mGraph.mNodes)
    {
        GraphLayout::GraphBlock block;
        block.entry = node.first;

        auto edgesItr = mGraph.mEdges.find(block.entry);
        if(edgesItr != mGraph.mEdges.end())
        {
            for(const auto& to : edgesItr->second)
            {
                block.edges.emplace_back(to);
                edges.insert(to);
            }
        }

        addBlock(std::move(block), node.second, node.first);
    }

    for(const auto& x : edges) {
        if(blockContent.find(x) != blockContent.end()) {
            // Already visited
            continue;
        }

        // Create fake node for an unknown destination
        GraphLayout::GraphBlock block;
        block.entry = x;
        addBlock(block, QString("unknown_%1").arg(RzHexString(x)), x);
    }

    computeGraphPlacement();

    // TODO: this doesn't seem to always work right away
    QTimer::singleShot(0, [this]
        {
            center();
        });
}
void GenericGraphView::blockClicked(GraphView::GraphBlock& block, QMouseEvent* event, QPoint pos)
{
    auto oldSelection = selectedBlock;
    SimpleTextGraphView::blockClicked(block, event, pos);
    if(selectedBlock != oldSelection)
        emit blockSelectionChanged(selectedBlock);
}

GraphDialog::GraphDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::GraphDialog)
{
    setWindowFlags(Qt::Tool);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->setupUi(this);
    qtRestoreGeometry(this);

    mGraphView = new GenericGraphView(this);
    ui->verticalLayout->addWidget(mGraphView);
}

GraphDialog::~GraphDialog()
{
    delete ui;
}

void GraphDialog::closeEvent(QCloseEvent* event)
{
    qtSaveGeometry(this);
    QDialog::closeEvent(event);
}
