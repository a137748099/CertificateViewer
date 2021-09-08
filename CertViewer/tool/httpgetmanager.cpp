#include "httpgetmanager.h"
#include <QNetworkAccessManager>
#include <QEventLoop>

HttpGetManager::HttpGetManager(QObject *parent):
    QObject(parent)
{
    m_pNetworkManager = new QNetworkAccessManager(this);
//    connect(m_pNetworkManager, &QNetworkAccessManager::finished, this, &HttpGetManager::replyFinish);

}

HttpGetManager::~HttpGetManager()
{
    delete m_pNetworkManager;
    m_pNetworkManager = nullptr;
}

void HttpGetManager::setUrl(const QString &url)
{
    m_url = url;
}

QByteArray HttpGetManager::httpGet()
{
    m_bError = false;
    QNetworkReply *reply = m_pNetworkManager->get(QNetworkRequest(QUrl(m_url)));
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

//    reply->waitForReadyRead(2000);
    QByteArray replyData;
    if(!m_bError) {
        replyData = reply->readAll();
    }
    reply->deleteLater();
    reply = nullptr;
//    qDebug() << __FUNCTION__ << "reply data" << replyData;
    return replyData;
}

void HttpGetManager::onError(QNetworkReply::NetworkError error)
{
    m_bError = true;
}
