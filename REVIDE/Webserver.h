#pragma once

#include <QThread>
#include <memory>
#include "httplib.h"

class Webserver : public QThread
{
    Q_OBJECT
public:
    Webserver(int port, QObject* parent = nullptr);
    ~Webserver();
    void run() override;
    void close();

signals:
    void hello(QString ip);
    void llvm(QString type, QString title, QByteArray data);

private:
    httplib::Server* mServer;
    int mPort;
};
