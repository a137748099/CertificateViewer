#ifndef GENERALWIDGET_H
#define GENERALWIDGET_H

#include <QWidget>

namespace Ui {
class GeneralWidget;
}

class GeneralWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralWidget(QWidget *parent = nullptr);
    ~GeneralWidget();

    void setExtusage(const std::string &text);
    void setSubjectInfo(const QList<std::string> &infos);
    void setIssuerInfo(const QList<std::string> &infos);
    void setValidPeridInfo(const QList<std::string> &infos);
    void setFpInfo(const QList<std::string> &infos, bool isSM2 = false);

    void clear();

protected:
    void initUi();
    void setObjectListText(const QObjectList &objList, const QList<std::string> &infos);

private:
    Ui::GeneralWidget *ui;
};

#endif // GENERALWIDGET_H
