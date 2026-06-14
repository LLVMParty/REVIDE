#pragma once

#include <unordered_map>
#include <type_traits>
#include <algorithm>
#include <vector>

#include <QDialog>

#include "widgets/SimpleTextGraphView.h"

struct GenericGraph
{
    ut64 mId = UT64_MAX; // unique id identifying this graph (used for caching)
    std::unordered_map<ut64, QString> mNodes;
    std::unordered_map<ut64, std::vector<ut64>> mEdges;

    GenericGraph() = default;
    explicit GenericGraph(ut64 id) : mId(id) { }

    void addNode(ut64 id, const QString& text, const std::vector<ut64>& edges = {})
    {
        mNodes.emplace(id, text);
        for(const auto& to : edges)
            addEdge(id, to);
    }

    void addEdge(ut64 from, ut64 to)
    {
        auto& edges = mEdges[from];
        if(std::find(edges.begin(), edges.end(), to) == edges.end())
            edges.push_back(to);
    }

    void clear()
    {
        mNodes.clear();
        mEdges.clear();
    }
};
static_assert(std::is_move_assignable_v<GenericGraph>);
static_assert(std::is_move_constructible_v<GenericGraph>);

class GenericGraphView : public SimpleTextGraphView
{
    Q_OBJECT

public:
    explicit GenericGraphView(QWidget *parent);

    void setGraph(const GenericGraph& graph)
    {
        // Do not rebuild the current graph unnecessarily
        if(graph.mId == mGraph.mId)
            return;

        mGraph = graph;
        refreshView();
    }

    void clear()
    {
        mGraph = GenericGraph();
        refreshView();
    }

signals:
    void blockSelectionChanged(ut64 blockId);

protected:
    void loadCurrentGraph() override;
    void drawBlock(QPainter &p, GraphView::GraphBlock &block, bool interactive) override;
    GraphView::EdgeConfiguration edgeConfiguration(GraphView::GraphBlock &from,
                                                   GraphView::GraphBlock *to,
                                                   bool interactive) override;
    void blockClicked(GraphView::GraphBlock &block, QMouseEvent *event, QPoint pos) override;

private:
    GenericGraph mGraph;
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
    GenericGraphView* graphView() { return mGraphView; }

protected:
    void closeEvent(QCloseEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;

private:
    Ui::GraphDialog* ui = nullptr;
    GenericGraphView* mGraphView = nullptr;
};
