#ifndef HTTPGETMANAGER_H
#define HTTPGETMANAGER_H

#include <QObject>
#include <QNetworkReply>

class QNetworkAccessManager;
class QNetworkReply;

class HttpGetManager : public QObject
{
    Q_OBJECT
public:
    explicit HttpGetManager(QObject *parent = nullptr);
    ~HttpGetManager();

    void setUrl(const QString &url);
    QByteArray httpGet();

signals:
    void sigGetFinish(const QByteArray &data);

private slots:
    void onError(QNetworkReply::NetworkError error);

private:
    QNetworkAccessManager *m_pNetworkManager = nullptr;
    QString m_url;
    bool m_bError = false;
};

#endif // HTTPGETMANAGER_H
