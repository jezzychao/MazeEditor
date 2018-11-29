#include "dlgconfirm.h"
#include "ui_dlgconfirm.h"

DlgConfirm::DlgConfirm(const QString &tips, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConfirm)
{
    ui->setupUi(this);
    ui->label->setText(tips);
}

DlgConfirm::~DlgConfirm()
{
    delete ui;
}
