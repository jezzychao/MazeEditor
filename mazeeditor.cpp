#include "mazeeditor.h"
#include "ui_mazeeditor.h"
#include "setbasicinfo.h"
#include "formmgr.h"
#include "msgcenter.h"
#include "basejsonhelper.h"
#include <QMessageBox>
#include "cusscene.h"
#include "dlgopenmaze.h"
#include "dlgsetoption.h"
#include "publishhelper.h"
#include "dlgconfirm.h"
#include "dlgsetstage.h"
#include "dlgruntest.h"

bool isCanUseTopBtn()
{
    return MazeHelper::getInstance()->isAlreadyExist();
}

mazeeditor::mazeeditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mazeeditor)
{
    ui->setupUi(this);
    ui->graphicsView->setEnabled(false);

    MazeHelper::getInstance()->load();
    MsgCenter::getInstance()->attach(key2str(MsgKeys::ConfirmOpenMaze), [&](const std::string &key, const BaseMsg &msg) {
        this->acceptNotify(key,msg);
    });
    MsgCenter::getInstance()->attach(key2str(MsgKeys::ConfirmModifyBasicInfo), [&](const std::string &key, const BaseMsg &msg) {
        this->acceptNotify(key,msg);
    });
    MsgCenter::getInstance()->attach(key2str(MsgKeys::OpenDlgSetOption), [&](const std::string &key, const BaseMsg &msg) {
        this->acceptNotify(key,msg);
    });
    MsgCenter::getInstance()->attach(key2str(MsgKeys::OpenDlgSetStage), [&](const std::string &key, const BaseMsg &msg) {
        this->acceptNotify(key,msg);
    });
}

mazeeditor::~mazeeditor()
{
    MsgCenter::getInstance()->detach(key2str(MsgKeys::OpenDlgSetOption));
    MsgCenter::getInstance()->detach(key2str(MsgKeys::ConfirmOpenMaze));
    MsgCenter::getInstance()->detach(key2str(MsgKeys::ConfirmModifyBasicInfo));
     MsgCenter::getInstance()->detach(key2str(MsgKeys::OpenDlgSetStage));
    delete ui;
}

void mazeeditor::acceptNotify(const std::string &key, const BaseMsg &msg)
{
    if(key == key2str(MsgKeys::ConfirmOpenMaze)){
        ui->graphicsView->setEnabled(true);
        ui->graphicsView->reset();
    }else if(key == key2str(MsgKeys::ConfirmModifyBasicInfo)){
        auto &newMsg = static_cast<const MsgCloseTipsDlg &>(msg);
        if(newMsg.status){
            ui->graphicsView->setEnabled(true);
            ui->graphicsView->reset();
        }else{
            qWarning("no operation");
        }
    }else if(key == key2str(MsgKeys::OpenDlgSetOption)){
        const auto &m = static_cast<const MsgInt&>(msg);
        DlgSetOption *dlg = new DlgSetOption(this);
        dlg->init(m.number);
        dlg->exec();
    }else if(key == key2str(MsgKeys::OpenDlgSetStage)){
        const auto &m = static_cast<const MsgInt&>(msg);
        DlgSetStage *dlg= new  DlgSetStage(this);
        dlg->init(m.number);
        dlg->exec();
    }
}

void mazeeditor::on_btn_new_clicked()
{
    FormMgr::getInstance()->open("setbasicinfo");
    MsgCenter::getInstance()->notify(key2str(MsgKeys::OpenSetBasicInfoForNew),MsgNull());
}

void mazeeditor::on_btn_open_clicked()
{
    DlgOpenMaze *dlg = new DlgOpenMaze(this);
    dlg->exec();
}

void mazeeditor::on_btn_set_clicked()
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

void mazeeditor::on_btn_publish_clicked()
{
    PublishHelper helper;
    if( helper.exe()){
        DlgConfirm *dlg = new DlgConfirm("发布数据成功",this);
        dlg->exec();
    }else{
        DlgConfirm *dlg = new DlgConfirm("发布数据失败！！！",this);
        dlg->exec();
    }
}

void mazeeditor::on_bnt_del_clicked()
{
    if(!isCanUseTopBtn()){
        QMessageBox::information(this, tr("信息"), tr("没有打开的迷宫"));
        qWarning("btn_save is invalid!!!");
        return;
    }
    DlgConfirm *dlg = new DlgConfirm("确认要删除该迷宫吗？",this);
    auto result = dlg->exec();
    if(result == QDialog::Accepted){
        auto mazeId = MazeHelper::getInstance()->getCurrMaze().id;
        MazeHelper::getInstance()->deleteMaze(mazeId);
        MazeHelper::getInstance()->setCurrMaze(0);
        MazeHelper::getInstance()->save();
         ui->graphicsView->clear();
         ui->graphicsView->setEnabled(false);
    }
}

void mazeeditor::on_pushButton_clicked()
{
    if(!isCanUseTopBtn()){
        QMessageBox::information(this, tr("信息"), tr("没有打开的迷宫"));
        qWarning("btn_save is invalid!!!");
        return;
    }
    DlgRunTest *pdlg = new DlgRunTest(MazeHelper::getInstance()->getSpMaze(),this);
    pdlg->show();
}
