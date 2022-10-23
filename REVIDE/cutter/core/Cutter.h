#pragma once

#include <cstdint>

#include <QObject>
#include <QPolygonF>

#include "CutterCommon.h"

// HACK: this is fake
class CutterCore : public QObject {
    Q_OBJECT

public:
    explicit CutterCore(QObject* parent);
    static CutterCore* instance();

    RVA getOffset() const;
    void seek(RVA addr);
    void seekPrev();
    QString cmdRawAt(const QString& cmd, RVA addr);

signals:
    void seekChanged(RVA addr);
    void graphOptionsChanged();
};

#define Core() CutterCore::instance()