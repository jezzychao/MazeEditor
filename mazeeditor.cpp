#include "mazeeditor.h"
#include "ui_mazeeditor.h"
#include "setbasicinfo.h"

mazeeditor::mazeeditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mazeeditor)
{
    ui->setupUi(this);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
}

mazeeditor::~mazeeditor()
{
    delete ui;
}

void mazeeditor::on_actionview_triggered()
{
         ui->scrollArea->hide();
         ui->scrollArea->show();
}

void mazeeditor::on_actionedit_triggered()
{

}

void mazeeditor::on_actionnew_triggered()
{
    ui->txt_logInfo->setText("on_actionnew_triggered");
    auto pdlg = new SetBasicInfo(this);
    pdlg->show();
}
