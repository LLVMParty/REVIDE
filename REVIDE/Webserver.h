#pragma once

#include <QThread>
#include <memory>

namespace httplib
{
    class Server;
}

class Webserver : public QThread
{
    Q_OBJECT
public:
    Webserver(QObject* parent = nullptr);
    void run() override;
    void close();

signals:
    void hello(QString ip);
    void llvm(QString type, QString title, QByteArray data);

private:
    std::unique_ptr<httplib::Server> mServer;
};

