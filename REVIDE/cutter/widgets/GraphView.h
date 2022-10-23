#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QObject>
#include <QPainter>
#include <QWidget>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QElapsedTimer>
#include <QHelpEvent>
#include <QGestureEvent>

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <memory>

#include "core/Cutter.h"
#include "widgets/GraphLayout.h"

#if defined(QT_NO_OPENGL) || QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
// QOpenGLExtraFunctions were introduced in 5.6
#    define CUTTER_NO_OPENGL_GRAPH
#endif

#ifndef CUTTER_NO_OPENGL_GRAPH
class QOpenGLWidget;
#endif

class GraphView : public QAbstractScrollArea
{
    Q_OBJECT

signals:
    void viewOffsetChanged(QPoint offset);
    void viewScaleChanged(qreal scale);

public:
    using GraphBlock = GraphLayout::GraphBlock;
    using GraphEdge = GraphLayout::GraphEdge;

    enum class Layout {
        GridNarrow,
        GridMedium,
        GridWide,
        GridAAA,
        GridAAB,
        GridABA,
        GridABB,
        GridBAA,
        GridBAB,
        GridBBA,
        GridBBB
#ifdef CUTTER_ENABLE_GRAPHVIZ
        ,
        GraphvizOrtho,
        GraphvizPolyline,
        GraphvizSfdp,
        GraphvizNeato,
        GraphvizTwoPi,
        GraphvizCirco
#endif
    };
    static std::unique_ptr<GraphLayout> makeGraphLayout(Layout layout, bool horizontal = false);

    struct EdgeConfiguration
    {
        QColor color = QColor(128, 128, 128);
        bool start_arrow = false;
        bool end_arrow = true;
        qreal width_scale = 1.0;
        Qt::PenStyle lineStyle = Qt::PenStyle::SolidLine;
    };

    explicit GraphView(QWidget *parent);
    ~GraphView() override;

    void showBlock(GraphBlock &block, bool anywhere = false);
    /**
     * @brief Move view so that area is visible.
     * @param rect Rectangle to show
     * @param anywhere - set to true for minimizing movement
     */
    void showRectangle(const QRect &rect, bool anywhere = false);
    /**
     * @brief Get block containing specified point logical coordinates.
     * @param p positionin graph logical coordinates
     * @return Block or nullptr if position is outside all blocks.
     */
    GraphView::GraphBlock *getBlockContaining(QPoint p);
    QPoint viewToLogicalCoordinates(QPoint p);
    QPoint logicalToViewCoordinates(QPoint p);

    void setGraphLayout(std::unique_ptr<GraphLayout> layout);
    GraphLayout &getGraphLayout() const { return *graphLayoutSystem; }
    void setLayoutConfig(const GraphLayout::LayoutConfig &config);

    void paint(QPainter &p, QPoint offset, QRect area, qreal scale = 1.0, bool interactive = true);

    void saveAsBitmap(QString path, const char *format = nullptr, double scaler = 1.0,
                      bool transparent = false);
    void saveAsSvg(QString path);

    void computeGraphPlacement();

    /**
     * @brief Remove duplicate edges and edges without target in graph.
     * @param graph
     */
    static void cleanupEdges(GraphLayout::Graph &graph);

protected:
    std::unordered_map<ut64, GraphBlock> blocks;
    /// image background color
    QColor backgroundColor = QColor(Qt::white);

    // Padding inside the block
    int block_padding = 16;

    void setCacheDirty() { cacheDirty = true; }

    void addBlock(GraphView::GraphBlock block);
    void setEntry(ut64 e);

    // Callbacks that should be overridden
    /**
     * @brief drawBlock
     * @param p painter object, not necesarily current widget
     * @param block
     * @param interactive - can be used for disabling elemnts during export
     */
    virtual void drawBlock(QPainter &p, GraphView::GraphBlock &block, bool interactive = true) = 0;
    virtual void blockClicked(GraphView::GraphBlock &block, QMouseEvent *event, QPoint pos);
    virtual void blockDoubleClicked(GraphView::GraphBlock &block, QMouseEvent *event, QPoint pos);
    virtual void blockHelpEvent(GraphView::GraphBlock &block, QHelpEvent *event, QPoint pos);
    virtual bool helpEvent(QHelpEvent *event);
    virtual void blockTransitionedTo(GraphView::GraphBlock *to);
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual EdgeConfiguration edgeConfiguration(GraphView::GraphBlock &from,
                                                GraphView::GraphBlock *to, bool interactive = true);
    virtual bool gestureEvent(QGestureEvent *event);
    /**
     * @brief Called when user requested context menu for a block. Should open a block specific
     * contextmenu. Typically triggered by right click.
     * @param block - the block that was clicked on
     * @param event - context menu event that triggered the callback, can be used to display context
     * menu at correct position
     * @param pos - mouse click position in logical coordinates of the drawing, set only if event
     * reason is mouse
     */
    virtual void blockContextMenuRequested(GraphView::GraphBlock &block, QContextMenuEvent *event,
                                           QPoint pos);

    bool event(QEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

    // Mouse events
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    int width = 0;
    int height = 0;
    bool scale_thickness_multiplier = false;

    void clampViewOffset();
    void setViewOffsetInternal(QPoint pos, bool emitSignal = true);
    void addViewOffset(QPoint move, bool emitSignal = true);

private:
    void centerX(bool emitSignal);
    void centerY(bool emitSignal);

    void paintGraphCache();

    bool checkPointClicked(QPointF &point, int x, int y, bool above_y = false);

    // Zoom data
    qreal current_scale = 1.0;

    QPoint offset = QPoint(0, 0);

    ut64 entry = 0;

    std::unique_ptr<GraphLayout> graphLayoutSystem;

    QPoint scrollBase;
    bool scroll_mode = false;

    bool useGL;

    /**
     * @brief pixmap that caches the graph nodes
     */
    QPixmap pixmap;

#ifndef CUTTER_NO_OPENGL_GRAPH
    uint32_t cacheTexture;
    uint32_t cacheFBO;
    QSize cacheSize;
    QOpenGLWidget *glWidget;
#endif

    /**
     * @brief flag to control if the cache is invalid and should be re-created in the next draw
     */
    bool cacheDirty = true;
    QSize getCacheSize();
    qreal getCacheDevicePixelRatioF();
    QSize getRequiredCacheSize();
    qreal getRequiredCacheDevicePixelRatioF();

    void beginMouseDrag(QMouseEvent *event);

public:
    QPoint getViewOffset() const { return offset; }
    void setViewOffset(QPoint offset);
    qreal getViewScale() const { return current_scale; }
    void setViewScale(qreal scale);

    void center();
    void centerX() { centerX(true); }
    void centerY() { centerY(true); }
};

#endif // GRAPHVIEW_H
