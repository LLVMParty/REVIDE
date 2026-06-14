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

void GenericGraphView::drawBlock(QPainter& p, GraphView::GraphBlock& block, bool interactive)
{
    const bool blockSelected = interactive && block.entry == selectedBlock;
    const auto oldSelectedBackground = disassemblySelectedBackgroundColor;
    disassemblySelectedBackgroundColor = disassemblyBackgroundColor;

    SimpleTextGraphView::drawBlock(p, block, interactive);
    disassemblySelectedBackgroundColor = oldSelectedBackground;

    if(blockSelected)
    {
        QPen pen(QColor("#61afef"), 2.0);
        pen.setCosmetic(true);
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);
        p.drawRect(QRectF(block.x, block.y, block.width, block.height).adjusted(1, 1, -1, -1));
    }
}

GraphView::EdgeConfiguration GenericGraphView::edgeConfiguration(GraphView::GraphBlock& from,
                                                                  GraphView::GraphBlock* to,
                                                                  bool interactive)
{
    EdgeConfiguration ec;
    ec.color = QColor("#4b5263");
    ec.start_arrow = false;
    ec.end_arrow = true;
    ec.width_scale = 1.25;

    auto edgesItr = mGraph.mEdges.find(from.entry);
    if(edgesItr != mGraph.mEdges.end())
    {
        const auto& edges = edgesItr->second;
        auto edgeItr = std::find(edges.begin(), edges.end(), to->entry);
        if(edges.size() == 2 && edgeItr != edges.end())
        {
            ec.color = (edgeItr == edges.begin()) ? QColor("#7f9868") : QColor("#b06464");
        }
        else if(edges.size() > 2)
        {
            ec.color = QColor("#56b6c2");
        }
    }

    if(interactive && (selectedBlock == from.entry || selectedBlock == to->entry))
    {
        ec.color = ec.color.lighter(125);
        ec.width_scale = 2.25;
    }

    return ec;
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

void GraphDialog::showEvent(QShowEvent* event)
{
    qDebug() << "showEvent()";
    QDialog::showEvent(event);
}

void GraphDialog::hideEvent(QHideEvent* event)
{
    qDebug() << "hideEvent()";
    QDialog::hideEvent(event);
}
