#include "httplib.h"
#include "Webserver.h"

using namespace httplib;

Webserver::Webserver(QObject* parent)
    : QThread(parent)
{
    mServer = new Server();

    mServer->Get("/hi", [this](const Request& req, Response& res) {
        emit hello(QString::fromStdString(req.remote_addr));
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
    if (!mServer->listen("localhost", 1867))
    {
        emit hello("listen :(");
    }
}

void Webserver::close()
{
    mServer->stop();
    while (mServer->is_running())
        QThread::msleep(10);
}
