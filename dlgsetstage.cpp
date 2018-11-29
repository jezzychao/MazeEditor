#include "dlgsetstage.h"
#include "ui_dlgsetstage.h"
#include <QStringList>
#include "basejsonhelper.h"
#include "msgcenter.h"
#include "formmgr.h"

DlgSetStage::DlgSetStage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSetStage),
    stageId(0)
{
    ui->setupUi(this);
    QStringList qls {
        "1:普通",
        "2:战斗",
        "3:采集",
        "4:障碍",
        "5:小游戏"
    };
    ui->cbx_type->addItems(qls);
}

DlgSetStage::~DlgSetStage()
{
    delete ui;
}

void DlgSetStage::init(int id)
{
    stageId = id;
    auto stage = MazeHelper::getInstance()->getStage(stageId);
    ui->txt_desc->setText(stage.desc);
    ui->txt_mappindId->setText(QString::number(stage.mappingId));
    ui->txt_name->setText(stage.name);
    ui->txt_sprite->setText(stage.sprite);
    ui->txt_storyId->setText(QString::number(stage.entryStoryId));
    ui->cbx_type->setCurrentIndex(stage.type > 0? stage.type - 1 :0);
}

void DlgSetStage::on_pushButton_clicked()
{
    qDebug("on_pushButton_clicked");
    auto stage = MazeHelper::getInstance()->getStage(stageId);
    stage.desc =  ui->txt_desc->toPlainText();
    stage.mappingId =  ui->txt_mappindId->toPlainText().toInt();
    stage.name =  ui->txt_name->toPlainText();
    stage.sprite =  ui->txt_sprite->toPlainText();
    stage.entryStoryId =  ui->txt_storyId->toPlainText().toInt();
    stage.type =  ui->cbx_type->currentIndex() + 1;

    MazeHelper::getInstance()->setStage(stage);
    MazeHelper::getInstance()->save();

     FormMgr::getInstance()->close("dlgsetstage");
     close();
}
