#include "dlgsetstage.h"
#include "ui_dlgsetstage.h"
#include <QStringList>
#include "basejsonhelper.h"
#include "msgcenter.h"
#include "dlgconfirm.h"

DlgSetStage::DlgSetStage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSetStage),
    stageId(0),
    allStageIds({-2})//第一个数表示空，用-2来标记
{
    ui->setupUi(this);
    QStringList qls {
        "1: 多选项",
        "2: NPC战斗",
        "3: 采集",
        "4: 障碍",
        "5: 小游戏",
        "6: 上传镜像",
        "7: 镜像战斗"
    };
    ui->cbx_type->addItems(qls);
    connect(ui->cbx_type,SIGNAL(currentIndexChanged(int)), this, SLOT(on_typeCombox_indexChanged(int)));
}

DlgSetStage::~DlgSetStage()
{
    delete ui;
}

void DlgSetStage::init(int id)
{
    on_typeCombox_indexChanged(0);
    stageId = id;

    if(stageId == MazeHelper::getInstance()->getCurrMaze().beginStageId){
        ui->pushButton_2->setVisible(false);
    }

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

void DlgSetStage::on_typeCombox_indexChanged(int index){
    disabledAllInput();
    switch (index) {
    case 0: initForCrossing();break;
    case 1:  initForNpcFight();break;
    case 2:  initForCollect();break;
    case 3:  initForBarrier();break;
    case 4:  initForMinigame();break;
    case 5:  initForUploading();break;
    case 6:  initForPlayerFight();break;
    }
}

void DlgSetStage::on_pushButton_clicked()
{
    qDebug("on_pushButton_clicked");
    auto result = checkForDuplication();
    if(!std::get<0>(result)){
        DlgConfirm *dlg = new DlgConfirm(QString("存在重复的后置连接，是否继续？"),this);
        auto  ret = dlg->exec();
        if(ret == QDialog::Rejected){
            return;
        }
    }

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
            bool isExisted = false;
            for(auto &id: linkIds){
                if(id == allStageIds[curIdx]){
                    isExisted = true;
                    break;
                }
            }
            if(!isExisted){
                linkIds.push_back(allStageIds[curIdx]);
            }
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
    MsgCenter::getInstance()->notify(key2str(MsgKeys::UpdateRectText),MsgInt(stageId));

    MazeHelper::getInstance()->save();
    close();
}

std::tuple<bool,QString> DlgSetStage::checkForDuplication()
{
    QVector<int> used;
    QVector<decltype(ui->cbx_link_1)> cbxs({ui->cbx_link_1,ui->cbx_link_2,ui->cbx_link_3});
    for(auto &cobom: cbxs){
        auto idx = cobom->currentIndex();
        bool isExisted = false;
        for(auto &v:used){
            if(idx == v && idx != 0){
                isExisted = true;
                break;
            }
        }
        if(isExisted){
            return std::make_tuple(false,QString("有多个重复的后置节点，请检查输入！"));
        }else{
            used.push_back(idx);
        }
    }
    return std::make_tuple(true,QString());
}

void DlgSetStage::on_pushButton_2_clicked()
{
    DlgConfirm *dlg = new DlgConfirm("确认删除该物体？",this);
    auto result = dlg->exec();
    if( result== QDialog::Accepted){
        MsgCenter::getInstance()->notify(key2str(MsgKeys::DeleteRect),MsgInt(stageId));
        close();
    }
}

void DlgSetStage::initForCrossing()
{
    ui->txt_name->setEnabled(true);
    ui->txt_desc->setEnabled(true);
    ui->txt_sprite->setEnabled(true);
    ui->txt_mappindId->setEnabled(false);
    ui->cbx_link_1->setEnabled(true);
    ui->cbx_link_2->setEnabled(true);
    ui->cbx_link_3->setEnabled(true);
}

void DlgSetStage::initForNpcFight()
{
    ui->txt_mappindId->setEnabled(true);
    ui->cbx_link_1->setEnabled(true);
    ui->lab_link_1->setText(QString("战斗完成后"));
    ui->lab_mappingId->setText(QString("映射 stage.xlsx 中的id"));
}

void DlgSetStage::initForCollect()
{
    this->initForNpcFight();
    ui->lab_link_1->setText(QString("采集完成后"));
}

void DlgSetStage::initForBarrier()
{
    this->initForNpcFight();
    ui->lab_link_1->setText(QString("障碍完成后"));
}

void DlgSetStage::initForMinigame()
{
    ui->txt_mappindId->setEnabled(true);
    ui->lab_mappingId->setText(QString("映射 minigames.xlsx 中的id"));
    ui->cbx_link_1->setEnabled(true);
    ui->cbx_link_2->setEnabled(true);
    ui->lab_link_1->setText(QString("小游戏失败后"));
    ui->lab_link_2->setText(QString("小游戏成功后"));
}

void DlgSetStage::initForUploading()
{
    initForNpcFight();
    ui->lab_link_1->setText(QString("链接到..."));
}

void DlgSetStage::initForPlayerFight()
{
    initForNpcFight();
    ui->lab_link_1->setText(QString("链接到..."));
}

///@brief 剧情i、备注和类型选择除外
void DlgSetStage::disabledAllInput()
{
    ui->txt_name->setEnabled(false);
    ui->txt_desc->setEnabled(false);
    ui->txt_sprite->setEnabled(false);
    ui->cbx_link_1->setEnabled(false);
    ui->cbx_link_2->setEnabled(false);
    ui->cbx_link_3->setEnabled(false);
    ui->lab_link_1->clear();
    ui->lab_link_2->clear();
    ui->lab_link_3->clear();
    ui->lab_mappingId->clear();
    ui->txt_mappindId->setEnabled(false);
}
