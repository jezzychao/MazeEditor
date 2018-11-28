#include "mazeeditor.h"
#include "ui_mazeeditor.h"
#include "setbasicinfo.h"
#include "formmgr.h"
#include "msgcenter.h"
#include "basejsonhelper.h"
#include <QMessageBox>

bool isCanUseTopBtn()
{
    return MazeHelper::getInstance()->isAlreadyExist();
}

mazeeditor::mazeeditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mazeeditor)
{
    MazeHelper::getInstance()->load();
    ui->setupUi(this);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    MsgCenter::getInstance()->attach(key2str(MsgKeys::ConfirmOpenMaze), [&](const std::string &key, const BaseMsg &msg) {
        this->acceptNotify(key,msg);
    });
    MsgCenter::getInstance()->attach(key2str(MsgKeys::ConfirmModifyBasicInfo), [&](const std::string &key, const BaseMsg &msg) {
        this->acceptNotify(key,msg);
    });
}

mazeeditor::~mazeeditor()
{
    MsgCenter::getInstance()->detach(key2str(MsgKeys::ConfirmOpenMaze));
    delete ui;
}

void mazeeditor::acceptNotify(const std::string &key, const BaseMsg &msg)
{
    if(key == key2str(MsgKeys::ConfirmOpenMaze)){
        clearView();
        initView();
    }else if(key == key2str(MsgKeys::ConfirmModifyBasicInfo)){
        auto &newMsg = static_cast<const MsgCloseTipsDlg &>(msg);
        if(newMsg.status){
            clearView();
            initView();
        }else{
            qWarning("no operation");
        }
    }
}

void mazeeditor::on_actionnew_triggered()
{
    FormMgr::getInstance()->open("setbasicinfo");
    MsgCenter::getInstance()->notify(key2str(MsgKeys::OpenSetBasicInfoForNew),MsgNull());
}

void mazeeditor::on_actionopen_triggered()
{
    FormMgr::getInstance()->open("dlgopenmaze");
}

void mazeeditor::on_btn_basicInfo_clicked()
{
    if(!isCanUseTopBtn()){
        QMessageBox::information(this, tr("信息"), tr("没有打开的迷宫"));
        qWarning("btn_basicInfo is invalid!!!");
       return;
    }
    FormMgr::getInstance()->open("setbasicinfo");
    MsgCenter::getInstance()->notify(key2str(MsgKeys::OpenSetBasicInfoForSet),MsgNull());
}

void mazeeditor::on_btn_save_clicked()
{
    if(!isCanUseTopBtn()){
         QMessageBox::information(this, tr("信息"), tr("没有打开的迷宫"));
        qWarning("btn_save is invalid!!!");
       return;
    }

    MazeHelper::getInstance()->save();
}

void mazeeditor::clearView(){
    ui->scrollArea->clearView();
}

void mazeeditor::initView(){
     ui->scrollArea->initView();
}


