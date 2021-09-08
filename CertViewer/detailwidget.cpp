#include "detailwidget.h"
#include "ui_detailwidget.h"
#include "tree/treemodel.h"
#include "x509certificate.h"
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QDebug>
#include <vector>
#include <algorithm>
#include <QFileDialog>
#include <QStandardPaths>

DetailWidget::DetailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DetailWidget)
{
    ui->setupUi(this);
    initUi();
}

DetailWidget::~DetailWidget()
{
    delete ui;
}

void DetailWidget::setX509(X509Certificate *x509)
{
    if(!x509 || x509->isNull()) {
        return;
    }
//    m_extMap.clear();

    auto commonName = QString::fromStdString(x509->subjectDisplyName());

    QStringList clickDataList, dataList;
    addNameData(dataList, commonName, clickDataList, "");
    addNameData(dataList, tr(" Certificate"), clickDataList, "");
    addNameData(dataList, tr("  Version"), clickDataList, QString("V%1").arg(x509->version() + 1));
    addNameData(dataList, tr("  SerialNum"), clickDataList, QString::fromStdString(x509->serialNumber()));
    addNameData(dataList, tr("  Signature Algorithm"), clickDataList, QString::fromStdString(x509->signAlgType()));
    addNameData(dataList, tr("  Issuer"), clickDataList, QString::fromStdString(x509->issuer()));
    addNameData(dataList, tr("  Subject"), clickDataList, QString::fromStdString(x509->subject()));
    addNameData(dataList, tr("  Validity"), clickDataList, "");
    addNameData(dataList, tr("   Not Before"), clickDataList, QString::fromStdString(x509->notBefor()));
    addNameData(dataList, tr("   Not After"), clickDataList, QString::fromStdString(x509->notAfter()));
    addNameData(dataList, tr("  Public Key"), clickDataList, "");
    addNameData(dataList, tr("   Public Key Algorithm"), clickDataList, QString::fromStdString(x509->publicKeyType()));
    addNameData(dataList, tr("   Public Key"), clickDataList, QString::fromStdString(x509->publicKeyValue()));
    addNameData(dataList, tr("  Extensions"), clickDataList, "");

    auto exts = x509->extensions();
    foreach (auto ext, exts) {
        auto extDisplayName = m_extMap.value(ext.oid(), "");
        qDebug() << __FUNCTION__ << "song" << extDisplayName;
        if(extDisplayName.isEmpty()) {
            //不常见的扩展项显示oid
            extDisplayName = QString("   %1").arg(ext.oid().c_str());
        }

        addNameData(dataList, extDisplayName, clickDataList, QString::fromStdString(ext.value()));
    }

    addNameData(dataList, tr(" Signature Algorithm"), clickDataList, QString::fromStdString(x509->signAlgType()));
    addNameData(dataList, tr(" Signature Value"), clickDataList, QString::fromStdString(x509->signValue()));
    m_pTreeModel = new TreeModel(dataList, clickDataList, commonName);

}

void DetailWidget::setCertChain(QList<X509Certificate> certs)
{
    this->clear();
    std::reverse(certs.begin(), certs.end());

    QTreeWidgetItem *parent = nullptr;
    QTreeWidgetItem *treeItem = nullptr;
    for(auto i = 0; i < certs.size(); ++i) {
        auto cert = certs.at(i);
        if(!parent) {
            treeItem = new QTreeWidgetItem(ui->treeWidget);
        }
        else {
            treeItem = new QTreeWidgetItem(parent);
        }

        treeItem->setText(0, QString::fromStdString(cert.subjectDisplyName()));
        parent = treeItem;
        setX509(&cert);
        m_modelMap.insert(treeItem, m_pTreeModel);
        m_certMap.insert(treeItem, cert);
    }

    ui->treeWidget->expandAll();
    ui->treeWidget->setCurrentItem(treeItem);

    ui->treeView->setModel(m_pTreeModel);
    ui->treeView->expandAll();
    ui->pushButton->setEnabled(true);
}

void DetailWidget::clear()
{
    auto topLevelItem = ui->treeWidget->topLevelItem(0);
    if(topLevelItem) {
        delete topLevelItem;
    }

    auto modelList = m_modelMap.values();
    qDeleteAll(modelList);

    m_modelMap.clear();
    m_certMap.clear();

    ui->textEdit->clear();
    ui->pushButton->setEnabled(false);
}


void DetailWidget::initUi()
{
    ui->treeWidget->header()->setVisible(false);
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, [=](QTreeWidgetItem *item, int){
        m_pTreeModel = m_modelMap.value(item);
        ui->treeView->setModel(m_pTreeModel);
        ui->treeView->expandAll();
    });

    ui->treeView->header()->setVisible(false);
    connect(ui->treeView, &QTreeView::clicked, this, [=](const QModelIndex &index){
        QVariant clickData = m_pTreeModel->clickData(index);
//        emit sigClicked(clickData.toString());
        ui->textEdit->setText(clickData.toString());
    });

    //常见的扩展项
    m_extMap = {{"2.5.29.15", tr("   Certificate Key Usage")},
                {"2.5.29.37", tr("   Extended Key Usage")},
                {"2.5.29.19", tr("   Basic Constraints")},
                {"2.5.29.14", tr("   Subject Key ID")},
                {"2.5.29.35", tr("   Authority Key ID")},
                {"1.3.6.1.5.5.7.1.1", tr("   Authority Information Access")},
                {"2.5.29.31", tr("   CRL Distribution Points")},
                {"2.5.29.32", tr("   Certificate Policies")},
                {"2.5.29.17", tr("   Subject Alternative Name")}};

    ui->textEdit->setFontFamily("simsun");
}

void DetailWidget::showEvent(QShowEvent *event)
{
    ui->pushButton->setEnabled(!m_certMap.isEmpty());
    QWidget::showEvent(event);
}

void DetailWidget::on_pushButton_clicked()
{
    auto item = ui->treeWidget->currentItem();
    auto cert = m_certMap.value(item);
    if(cert.isNull()) {
        return;
    }

    QString certName = item->text(0).section(":", -1).trimmed();
    QString defaultFilePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + QString("/%1").arg(certName);
    QString fileName = QFileDialog::getSaveFileName(nullptr,
          tr("Export Certificate"), defaultFilePath, tr("DER (*.cer *.crt);;PEM (*.pem)"));
    qDebug() << __FUNCTION__ << "song" << fileName;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "open file failed";
        return;
    }

    QByteArray certData;
    if(fileName.endsWith("pem")) {
        certData = QByteArray::fromStdString(cert.toPem());
    }
    else {
        certData = QByteArray::fromStdString(cert.toDer());
    }
    /*qint64 count = */file.write(certData);
    file.close();
}

void DetailWidget::addNameData(QStringList &dataList, const QString &data, QStringList &clickDataList, const QString &clickData)
{
    clickDataList.append(clickData);
    dataList.append(data);
}
