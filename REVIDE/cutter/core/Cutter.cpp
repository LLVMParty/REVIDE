#include "Cutter.h"

static CutterCore *uniqueInstance = nullptr;

CutterCore::CutterCore(QObject *parent) : QObject(parent)
{
    if (uniqueInstance) {
        throw std::logic_error("Only one instance of CutterCore must exist");
    }
    uniqueInstance = this;
}

CutterCore *CutterCore::instance()
{
    return uniqueInstance;
}

RVA CutterCore::getOffset() const
{
    qDebug() << "getOffset()";
    return 0;
}

void CutterCore::seek(RVA addr)
{
    qDebug() << QString("seek(%1)").arg(RzAddressString(addr));
}

void CutterCore::seekPrev()
{
    qDebug() << "seekPrev()";
}

QString CutterCore::cmdRawAt(const QString &cmd, RVA addr)
{
    qDebug() << QString("cmdRawAt(%1, %2)").arg(cmd, RzAddressString(addr));
    return QString();
}
