#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class QTabWidget;
class GeneralWidget;
class DetailWidget;
class QMenuBar;
class X509Certificate;
class HttpGetManager;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void openFile(const QString &filePaht);

protected:
    void initUi();
    void resizeEvent(QResizeEvent *event) override;

private:
    void downloadCertChain(X509Certificate *x509, QList<X509Certificate> &certList);
    QString getDownloadUrl(X509Certificate *x509);

private slots:
    void onOpenFile();
    void onCloseFile();

private:
    QTabWidget *m_pTabWidget = nullptr;
    GeneralWidget *m_pGeneralWidget = nullptr;
    DetailWidget *m_pDetailWidget = nullptr;
    QMenuBar *m_pMenuBar = nullptr;

    X509Certificate *m_pX509 = nullptr;
    HttpGetManager *m_pGetManager = nullptr;
};
#endif // MAINWIDGET_H
