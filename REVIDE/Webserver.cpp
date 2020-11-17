#include "Webserver.h"

using namespace httplib;

Webserver::Webserver(int port, QObject* parent)
    : QThread(parent)
    , mPort(port)
{
    mServer = new Server();

    mServer->Get("/hi", [this](const Request& req, Response& res) {
        emit hello(tr("Hello from %1").arg(QString::fromStdString(req.remote_addr)));
    });

    mServer->Post("/llvm", [this](const Request& req, Response& res) {
        QString type, title;
        if (!req.has_param("type"))
        {
            res.status = 422;
            res.set_content("Missing parameter 'type'", "text/plain");
            return;
        }
        type = QString::fromStdString(req.get_param_value("type"));
        if (req.has_param("title"))
            title = QString::fromStdString(req.get_param_value("title"));

        auto body = QByteArray::fromStdString(req.body);
        body = QByteArray::fromBase64(body);

        emit llvm(type, title, body);
    });

    // TODO: VTIL symbolic expression
    // https://blog.can.ac/2020/04/11/writing-an-optimizing-il-compiler-for-dummies-by-a-dummy/
}

Webserver::~Webserver()
{
    delete mServer;
}

void Webserver::run()
{
    if (!mServer->listen("localhost", mPort))
    {
        emit hello(tr("Failed to listen on localhost:%1").arg(mPort));
    }
}

void Webserver::close()
{
    mServer->stop();
    while (mServer->is_running())
        QThread::msleep(10);
}
