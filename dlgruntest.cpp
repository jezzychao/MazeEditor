#include "dlgruntest.h"
#include "ui_dlgruntest.h"
#include "basejsonhelper.h"
#include <QMap>
#include <QMessageBox>

DlgRunTest::DlgRunTest( std::shared_ptr<MazeData> sp,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRunTest),
    sp_maze(sp),
    currStageId(sp->beginStageId)
{
    ui->setupUi(this);
    refresh();
}

DlgRunTest::~DlgRunTest()
{
    delete ui;
}

void DlgRunTest::refresh()
{



    ui->lab_name->clear();
    ui->lab_id->clear();
    ui->lab_desc->clear();
    ui->lab_sprite->clear();
    ui->lab_storyId->clear();
    ui->lab_type->clear();
    ui->lab_remark->clear();

    const auto &stage = sp_maze->stages[currStageId];
    ui->lab_name->setText(stage.name);
    ui->lab_remark->setText("备注："+stage.remark);
    ui->lab_id->setText("ID：" + QString::number(stage.id));
    ui->lab_desc->setText("描述："+stage.desc);
    ui->lab_sprite->setText("图片："  + stage.sprite);
    ui->lab_storyId->setText("进入剧情ID：" + QString::number(stage.entryStoryId));
    QMap<int, QString> type2name{
        {0,"未设定类型"},
        {1,"多选项"},
        {2,"战斗"},
        {3,"采集"},
        {4,"障碍"},
        {5,"小游戏"},
        {6,"上传镜像"},
        {7,"镜像战斗"}
    };
    ui->lab_type->setText("类型：" + type2name[stage.type]);
    for(std::size_t i = 0;i != 3;++i){
        auto pbtn = ui->wid_core->findChild<QPushButton *>("pbtn_" + QString::number(i));
        pbtn->setVisible(false);
    }
    switch(stage.type)
    {
    case 1:
    {
        int idx=0;
        for(auto it = stage.options.cbegin();it != stage.options.cend();++it){
            auto pbtn = ui->wid_core->findChild<QPushButton *>("pbtn_" + QString::number(idx));
            pbtn->setVisible(true);
            pbtn->setText(it->text);
            ++idx;
        }
    }
        break;
    case 2:
        ui->pbtn_0->setVisible(true);
        ui->pbtn_0->setText("战斗");
        break;
    case 3:
        ui->pbtn_0->setVisible(true);
        ui->pbtn_0->setText("采集");
        break;
    case 4:
        ui->pbtn_0->setVisible(true);
        ui->pbtn_0->setText("障碍");
        break;
    case 5:
        ui->pbtn_0->setVisible(true);
        ui->pbtn_0->setText("失败");
        ui->pbtn_1->setVisible(true);
        ui->pbtn_1->setText("成功");
        break;
    case 6:
        ui->pbtn_0->setVisible(true);
        ui->pbtn_0->setText("继续探索");
        ui->pbtn_1->setVisible(true);
        ui->pbtn_1->setText("上传镜像");
        break;
    case 7:
        ui->pbtn_1->setVisible(true);
        ui->pbtn_1->setText("挑战镜像");
        ui->pbtn_0->setVisible(true);
        ui->pbtn_0->setText("继续探索");
        break;
    default:
        break;
    }

    auto result = MazeHelper::getInstance()->checkStageIsValid(stage);
    if(!std::get<0>(result)){
        QMessageBox::information(this, tr("信息"), std::get<1>(result));
    }
}

void DlgRunTest::doClicked(int index)
{
    const auto &stage = sp_maze->stages[currStageId];
    int nextStageId = -1;
    switch(stage.type)
    {
    case 1:
    {
        int idx = 0, id = 0;
        for(auto it = stage.options.cbegin();it != stage.options.cend();++it){
            if(idx == index){
                id = it->id;
                break;
            }
            ++idx;
        }
        auto &option =  stage.options[id];
        nextStageId = option.linkStageId;
        break;
    }
    case 2:
    case 3:
    case 4:
    {
        auto &option =  stage.options.first();
        nextStageId = option.linkStageId;
        break;
    }
    case 5:
    {
        nextStageId = index  == 0?stage.options.first().linkStageId:  stage.options.last().linkStageId;
        break;
    }
    case 7:
    case 6:
    {
        if(index == 0){
            auto &option =  stage.options.first();
            nextStageId = option.linkStageId;
        }
        else{
            QMessageBox::information(this, tr("信息"), tr(stage.type == 7 ?"镜像战斗":"上传镜像"));
            return;
        }
        break;
    }
    default:
        break;
    }

    if(nextStageId == -1){
        QMessageBox::information(this, tr("信息"), tr("没有配置后续的stage"));
    }else if(nextStageId == 0){
        QMessageBox::information(this, tr("信息"), tr("已经到达出口"));
    }else{
        switch2next(nextStageId);
    }
}

void DlgRunTest::switch2next(int nextStageId)
{
    currStageId = nextStageId;
    refresh();
}

void DlgRunTest::on_pbtn_0_clicked()
{
    doClicked(0);
}

void DlgRunTest::on_pbtn_1_clicked()
{
    doClicked(1);
}

void DlgRunTest::on_pbtn_2_clicked()
{
    doClicked(2);
}

void DlgRunTest::on_pushButton_clicked()
{
    sp_maze = MazeHelper::getInstance()->getSpMaze();
    refresh();
}
