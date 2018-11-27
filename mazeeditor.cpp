#include "mazeeditor.h"
#include "ui_mazeeditor.h"
#include "setbasicinfo.h"
#include "formmgr.h"
#include "msgcenter.h"
#include "basejsonhelper.h"

mazeeditor::mazeeditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mazeeditor)
{
    MazeHelper::getInstance()->load();
    ui->setupUi(this);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    MsgCenter::getInstance()->attach(key2str(MsgKeys::ConfirmModifyBasicInfo), [&](const std::string &key, const BaseMsg &msg) {
        this->acceptNotify(key,msg);
    });
}

mazeeditor::~mazeeditor()
{
     MsgCenter::getInstance()->detach(key2str(MsgKeys::ConfirmModifyBasicInfo));
    delete ui;
}

void mazeeditor::acceptNotify(const std::string &key, const BaseMsg &msg)
{
    if(key == key2str(MsgKeys::ConfirmModifyBasicInfo)){

    }
}

void mazeeditor::on_actionview_triggered()
{

}

void mazeeditor::on_actionedit_triggered()
{

}

void mazeeditor::on_actionnew_triggered()
{
    FormMgr::getInstance()->open("setbasicinfo");
    MsgSetBasicInfo msg;
    msg.isEmpty = true;
    MsgCenter::getInstance()->notify(key2str(MsgKeys::OpenSetBasicInfo), msg);
}

void mazeeditor::on_btn_basicInfo_clicked()
{
    FormMgr::getInstance()->open("setbasicinfo");
    MsgSetBasicInfo msg;
    msg.isEmpty = false;
    MsgCenter::getInstance()->notify(key2str(MsgKeys::OpenSetBasicInfo), msg);
}
