#include "generalwidget.h"
#include "ui_generalwidget.h"
#include <QDebug>

GeneralWidget::GeneralWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeneralWidget)
{
    ui->setupUi(this);
    initUi();
}

GeneralWidget::~GeneralWidget()
{
    delete ui;
}

void GeneralWidget::setExtusage(const std::string &text)
{
    ui->usageLineEdit->setText(text.c_str());
}

void GeneralWidget::setSubjectInfo(const QList<std::string> &infos)
{
    if(infos.size() != 3) {
        return;
    }

    auto childList = ui->subjectGroupBox->children();
    setObjectListText(childList, infos);
}

void GeneralWidget::setIssuerInfo(const QList<std::string> &infos)
{
    if(infos.size() != 3) {
        return;
    }

    auto childList = ui->issuerGroupBox->children();
    setObjectListText(childList, infos);
}

void GeneralWidget::setValidPeridInfo(const QList<std::string> &infos)
{
    if(infos.size() != 2) {
        return;
    }

    auto childList = ui->validDateGroupBox->children();
    setObjectListText(childList, infos);
}

void GeneralWidget::setFpInfo(const QList<std::string> &infos, bool isSM2)
{
    if(infos.size() != 2) {
        return;
    }

    auto childList = ui->fpGroupBox->children();
    setObjectListText(childList, infos);

    ui->label_10->setText(isSM2 ? u8"SM3指纹" : u8"SHA-256指纹");
}

void GeneralWidget::clear()
{
    ui->usageLineEdit->clear();
    auto childList = this->children();
    foreach (auto child, childList) {
        auto className = child->metaObject()->className();
        if(strcmp(className, "QGroupBox") == 0) {
            auto groupBox = qobject_cast<QGroupBox *>(child);
            auto groupChildList = groupBox->children();
            foreach (auto groupChild, groupChildList) {
                auto groupClassName = groupChild->metaObject()->className();
                if(strcmp(groupClassName, "QLineEdit") == 0) {
                    auto lineEdit = qobject_cast<QLineEdit *>(groupChild);
                    lineEdit->clear();
                }
            }
        }
    }
}


void GeneralWidget::initUi()
{
//    ui->usageLineEdit->setReadOnly(true);
    auto childList = this->children();
    foreach (auto child, childList) {
        auto className = child->metaObject()->className();
        if(strcmp(className, "QGroupBox") == 0) {
            auto groupBox = qobject_cast<QGroupBox *>(child);
            groupBox->setStyleSheet("border:none");
            QFont ft;
            ft.setBold(true);
            groupBox->setFont(ft);
        }
        else if(strcmp(className, "QLineEdit") == 0) {
            auto lineedit = qobject_cast<QLineEdit *>(child);
            lineedit->setStyleSheet("border:none");
            lineedit->setReadOnly(true);
        }
    }
}

void GeneralWidget::setObjectListText(const QObjectList &objList, const QList<std::string> &infos)
{
    auto index = 0;
    foreach (auto child, objList) {
        auto className = child->metaObject()->className();
        if(strcmp(className, "QLineEdit") == 0) {
            auto lineedit = qobject_cast<QLineEdit *>(child);
            lineedit->setText(infos.at(index++).c_str());
            if(lineedit->text().isEmpty()) {
                lineedit->setText(tr("<Not included in the certificate>"));
            }
            lineedit->setCursorPosition(0);
        }
    }
}
