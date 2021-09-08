#include "mainwidget.h"
#include "generalwidget.h"
#include "detailwidget.h"
#include "x509certificate.h"
#include "tool/httpgetmanager.h"
#include <QTabWidget>
#include <QDebug>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    initUi();
}

MainWidget::~MainWidget()
{
    delete m_pGetManager;
}

void MainWidget::initUi()
{
    this->setWindowTitle(tr("Certificate Viewer"));

    m_pMenuBar = new QMenuBar(this);
    auto fileMenu = m_pMenuBar->addMenu(tr("&File"));
    auto helpMenu = m_pMenuBar->addMenu(tr("&Help"));

    auto openAct = fileMenu->addAction(tr("&Open"));
    connect(openAct, &QAction::triggered, this, &MainWidget::onOpenFile);

//    auto recentAct = fileMenu->addAction(tr("Recently opened files"));

    auto closeAct = fileMenu->addAction(tr("&Close"));
    connect(closeAct, &QAction::triggered, this, &MainWidget::onCloseFile);

    auto existAct = fileMenu->addAction(tr("&Exist"));
    connect(existAct, &QAction::triggered, this, [=]{
        this->close();
    });

    auto whatAct = helpMenu->addAction(tr("&What it is?"));
    connect(whatAct, &QAction::triggered, this, [=]{
//        QDesktopServices::openUrl(QUrl(QLatin1String(“http://buer.website“)));
    });

    auto aboutAct = helpMenu->addAction(tr("&About"));
    connect(aboutAct, &QAction::triggered, this, [=]{
        QMessageBox::about(this, tr("About"), tr("Certificate Viewer V1.0.0.0"));
    });

    m_pTabWidget = new QTabWidget(this);
    m_pTabWidget->setStyleSheet("QTabBar::tab {min-width:100px;min-height:27px;}");

    m_pGeneralWidget = new GeneralWidget;
    m_pTabWidget->addTab(m_pGeneralWidget, tr("General Info(G)"));

    m_pDetailWidget = new DetailWidget;
    m_pTabWidget->addTab(m_pDetailWidget, tr("Detail Info(D)"));


//    m_pDetailWidget->setX509(m_pX509);

    m_pGetManager = new HttpGetManager;


}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    this->setFixedSize(544, 685);
    m_pMenuBar->resize(this->width(), 23);
    m_pTabWidget->setGeometry(0, m_pMenuBar->height(), this->width(), this->height() - m_pMenuBar->height());
//    m_pTabWidget->resize(this->size());
    QWidget::resizeEvent(event);
}

void MainWidget::downloadCertChain(X509Certificate *x509, QList<X509Certificate> &certList)
{
    if(!x509) {
        return;
    }

    certList.append(*x509);
    auto downloadUrl = getDownloadUrl(x509);
    if(downloadUrl.isEmpty()) {
        auto exts = x509->extensions();
        string authKeyID, subjectKeyID;
        foreach (auto ext, exts) {
            if(ext.oid() == "2.5.29.35") {
                auto map = ext.toMap();
                auto iter = map.find("keyid");
                if(iter != map.end()) {
                    authKeyID = iter->second;
                }
            }
            else if(ext.oid() == "2.5.29.14") {
                subjectKeyID = ext.toString();
            }
        }

        if(authKeyID != subjectKeyID) {
            vector<X509Certificate> sysCerts = X509Certificate::systemCaCertificates();
            foreach (auto sysCert, sysCerts) {
                auto sysExts = sysCert.extensions();
                foreach (auto ext, sysExts) {
                    if(ext.oid() == "2.5.29.14") {
                        auto subKeyID = ext.toString();
                        if(subKeyID == authKeyID) {
                            certList.append(sysCert);
//                            hasRoot = true;
                            return;
//                            qDebug() << __FUNCTION__ << "song" << "root cert" << sysCert.toPem();
                        }
                    }
                }
            }
        }
//        else {
//            hasRoot = true;
//        }

        return;
    }

    m_pGetManager->setUrl(downloadUrl);
    QByteArray data = m_pGetManager->httpGet();

    if(!data.isEmpty()) {
        auto caCert = X509Certificate(data.data(), data.size());
        downloadCertChain(&caCert, certList);
    }
}

QString MainWidget::getDownloadUrl(X509Certificate *x509)
{
    if(!x509) {
        return QString();
    }

    QString url;
    auto exts = x509->extensions();
    foreach (auto ext, exts) {
        if(ext.oid() == "1.3.6.1.5.5.7.1.1") {
            auto caMap = ext.toMap();
            auto iter = caMap.find("CA Issuers - URI");
            if(iter != caMap.end()) {
                url = QString::fromStdString(iter->second);
            }
        }
    }

    return url;
}

void MainWidget::onOpenFile()
{
    auto fileName = QFileDialog::getOpenFileName(this,
         tr("Open File"), "", tr("Cert Files (*.cer *.crt *.pem)"));

    if(fileName.isEmpty()) {
        return;
    }

    if(m_pX509) {
        delete m_pX509;
    }
    m_pX509 = new X509Certificate(fileName.toStdString().c_str(), 0);
    list<string> subjectInfos = {m_pX509->subjectDisplyName(),
                                 m_pX509->subjectInfo(X509Certificate::Organization),
                                 m_pX509->subjectInfo(X509Certificate::OrganizationalUnitName)};
    m_pGeneralWidget->setSubjectInfo(QList<string>::fromStdList(subjectInfos));

    list<string> issuerInfos = {m_pX509->issuerDisplyName(),
                               m_pX509->issuerInfo(X509Certificate::Organization),
                               m_pX509->issuerInfo(X509Certificate::OrganizationalUnitName)};
    m_pGeneralWidget->setIssuerInfo(QList<string>::fromStdList(issuerInfos));

    list<string> validExpirList = {m_pX509->notBefor(), m_pX509->notAfter()};
    m_pGeneralWidget->setValidPeridInfo(QList<string>::fromStdList(validExpirList));

    auto sigAlg = m_pX509->signAlgType();
    bool sm3WithSm2 = sigAlg == "SM3WithSM2Encryption";
    list<string> fpList = {m_pX509->digest(X509Certificate::Hash_Sha1),
                           sm3WithSm2 ? m_pX509->digest(X509Certificate::Hash_SM3) : m_pX509->digest(X509Certificate::Hash_Sha256)};
    m_pGeneralWidget->setFpInfo(QList<string>::fromStdList(fpList), sm3WithSm2);

    auto exts = m_pX509->extensions();
    foreach (auto ext, exts) {
        if(ext.oid() == "2.5.29.37") {
            m_pGeneralWidget->setExtusage(ext.value());
        }
    }

    QList<X509Certificate> certList;
    downloadCertChain(m_pX509, certList);
    qDebug() << __FUNCTION__ << "song" << certList.size();
    m_pDetailWidget->setCertChain(certList);
}

void MainWidget::onCloseFile()
{
    m_pGeneralWidget->clear();
    m_pDetailWidget->clear();
}

