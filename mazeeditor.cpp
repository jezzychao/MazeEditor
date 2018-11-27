#include "mazeeditor.h"
#include "ui_mazeeditor.h"
#include "setbasicinfo.h"
#include "formmgr.h"
#include "msgcenter.h"
#include "basejsonhelper.h"

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
            qWarning("重置视图界面");
            clearView();
            initView();
        }else{
            qWarning("不需要操作视图界面");
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
        qWarning("配置按钮无效");
       return;
    }
    FormMgr::getInstance()->open("setbasicinfo");
    MsgCenter::getInstance()->notify(key2str(MsgKeys::OpenSetBasicInfoForSet),MsgNull());
}

void mazeeditor::on_btn_save_clicked()
{
    if(!isCanUseTopBtn()){
        qWarning("保存按钮无效");
       return;
    }
    qWarning("保存迷宫数据");
    MazeHelper::getInstance()->save();
}

void mazeeditor::clearView(){
    qWarning("清空显示");
}

void mazeeditor::initView(){
    auto &maze = MazeHelper::getInstance()->getCurrMaze();
    qWarning("显示迷宫：%s",maze.get().name.toStdString().c_str());
}


