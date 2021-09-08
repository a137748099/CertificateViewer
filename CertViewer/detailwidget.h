#ifndef DETAILWIDGET_H
#define DETAILWIDGET_H

#include <QWidget>
#include <QMap>

namespace Ui {
class DetailWidget;
}

class TreeModel;
class X509Certificate;
class QTreeWidgetItem;

class DetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetailWidget(QWidget *parent = nullptr);
    ~DetailWidget();

    void setX509(X509Certificate *x509);
    void setCertChain(QList<X509Certificate> certs);

    void clear();

protected:
    void initUi();
    void showEvent(QShowEvent *event) override;

private slots:
    void on_pushButton_clicked();

private:
    void addNameData(QStringList &dataList, const QString &data, QStringList &clickDataList, const QString &clickData);

private:
    Ui::DetailWidget *ui;
    TreeModel *m_pTreeModel = nullptr;
    QMap<std::string, QString> m_extMap;
    QMap<QTreeWidgetItem *, TreeModel *> m_modelMap;
    QMap<QTreeWidgetItem *, X509Certificate> m_certMap;

};

#endif // DETAILWIDGET_H
