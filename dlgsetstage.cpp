#include "dlgsetstage.h"
#include "ui_dlgsetstage.h"
#include <QStringList>
#include "basejsonhelper.h"
#include "msgcenter.h"

DlgSetStage::DlgSetStage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSetStage),
    stageId(0),
    allStageIds({-2})//第一个数表示空，用-2来标记
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
    ui->txt_remark->setText(stage.remark);
    ui->cbx_type->setCurrentIndex(stage.type > 0? stage.type - 1 :0);

    //读取连接选项
    QVector<decltype(ui->cbx_link_1)> cbxs({ui->cbx_link_1,ui->cbx_link_2,ui->cbx_link_3});
    auto infos = MazeHelper::getInstance()->getStageInfos({stageId});
    QStringList qsl({"none"});
    for(auto it = infos.cbegin();it != infos.cend();++it)
    {
        allStageIds.push_back(it.key());
        qsl.push_back(QString(QString::number(it.key()) + ":" + it.value()));
    }

    QVector<int> linkIdxs;
    for(auto nextId: stage.nextStageIds){
        auto idx = allStageIds.indexOf(nextId);
        linkIdxs.push_back(idx);
    }

    for(auto it = cbxs.cbegin();it != cbxs.cend();++it){
        (*it)->addItems(qsl);
        auto setIdx = it-cbxs.cbegin() < linkIdxs.size()? linkIdxs[it-cbxs.cbegin()]:0;
        (*it)->setCurrentIndex(setIdx);
    }
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
    stage.remark = ui->txt_remark->toPlainText();

    //写入连接选项
    QVector<int> linkIds;
    QVector<decltype(ui->cbx_link_1)> cbxs({ui->cbx_link_1,ui->cbx_link_2,ui->cbx_link_3});
    for(auto &cbx:cbxs){
        auto curIdx = cbx->currentIndex();
        if(curIdx != 0){
            linkIds.push_back(allStageIds[curIdx]);
        }
    }
    stage.nextStageIds = linkIds;

    //add or delete
    QVector<int> addOpIds,delOpIds, addLinkIds;
    for(auto &op :stage.options){
        bool isExist = false;
        for(auto linkId: stage.nextStageIds){
            if(linkId == op.linkStageId){
                isExist = true;
                break;
            }
        }
        if(!isExist){
            delOpIds.push_back(op.id);
        }
    }
    for(auto linkId: stage.nextStageIds){
        bool isExisted = false;
        for(auto &op: stage.options){
            if(op.linkStageId == linkId){
                isExisted = true;
                break;
            }
        }
        if(!isExisted){
            addLinkIds.push_back(linkId);
        }
    }
    for(auto opId: delOpIds){
        stage.options.remove(opId);
    }
    for(auto linkId: addLinkIds){
        MazeOption option;
        option.id = MazeHelper::getInstance()->genNewOptionId(stage);
        option.linkStageId = linkId;
        stage.options.insert(option.id,option);
        addOpIds.push_back(option.id);
    }

    MazeHelper::getInstance()->setStage(stage);

    MsgResetArrows msg;
    msg.stageId = stage.id;
    msg.delOpIds = delOpIds;
    for(int i = 0;i != addLinkIds.size();++i){
        msg.opIdLinkToStageId.insert(addOpIds[i],addLinkIds[i]);
    }
    MsgCenter::getInstance()->notify(key2str(MsgKeys::ResetNextArrows),msg);

    MazeHelper::getInstance()->save();
    close();
}
