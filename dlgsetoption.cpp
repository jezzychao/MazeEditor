#include "dlgsetoption.h"
#include "ui_dlgsetoption.h"
#include "basejsonhelper.h"
#include "msgcenter.h"
#include <QMessageBox>

namespace  {
    const  QStringList attrs{
        "none",
        "hunger",       //饥饿
        "health",       //健康
        "hp",           //生命值
        "mentality",    //精神
        "waterPlague",  //水疫程度
        "mood"        //心情
    };
}

DlgSetOption::DlgSetOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSetOption),
    currOptionId(0)
{
    ui->setupUi(this);
    for(std::size_t i = 0;i != 5;++i){
        auto pcb = ui->attrs->findChild<QComboBox *>("cmb_attr_" + QString::number(i +1));
        pcb->addItems(attrs);
    }
}

DlgSetOption::~DlgSetOption()
{
    delete ui;
}

void DlgSetOption::init(int optionId)
{
    qDebug("open dlgsetoption id: %d",optionId);
    currOptionId = optionId;
    auto option = MazeHelper::getInstance()->getOption(optionId);
    ui->cebox->setCheckState(option.isonlyonce?Qt::CheckState::Checked:Qt::CheckState::Unchecked);
    ui->txt_remark->setText(option.remark);
    ui->txt_cond->setText(option.activecond);
    ui->txt_tips->setText(option.disabledTips);
    ui->txt_text->setText(option.text);
    for(int i = 0;i != 5;++i){
        auto sidx =  QString::number(i +1);
        auto pcb = ui->attrs->findChild<QComboBox *>("cmb_attr_" +sidx);
        auto pRoles = ui->attrs->findChild<QTextEdit *>("txt_role_" +sidx);
        auto pDetal = ui->attrs->findChild<QTextEdit *>("txt_delta_" +sidx);
        if(i < option.attrsChanged.size()){
            auto sval = option.attrsChanged[i];
            auto s = sval.split(";");
            pcb->setCurrentIndex(attrs.indexOf(s[1]));
            pRoles->setText(s[0]);
            pDetal->setText(s[2]);
        }else{
            pcb->setCurrentIndex(0);
            pRoles->clear();
            pDetal->clear();
        }
    }

    QStringList qls;
    for(auto eventId: option.events){
        qls.push_back(QString::number(eventId));
    }
    ui->txt_events->setText(qls.join(";"));
}

void DlgSetOption::on_pushButton_clicked()
{
    auto option = MazeHelper::getInstance()->getOption(currOptionId);
    option.text = ui->txt_text->toPlainText();
    option.remark = ui->txt_remark->toPlainText();
    option.activecond = ui->txt_cond->toPlainText();
    option.disabledTips = ui->txt_tips->toPlainText();
    option.isonlyonce = ui->cebox->checkState() == Qt::CheckState::Checked;

    option.attrsChanged.clear();
    for(int i = 0;i != 5;++i){
        auto sidx =  QString::number(i +1);
        auto pcb = ui->attrs->findChild<QComboBox *>("cmb_attr_" +sidx);
        auto pRoles = ui->attrs->findChild<QTextEdit *>("txt_role_" +sidx);
        auto pDetal = ui->attrs->findChild<QTextEdit *>("txt_delta_" +sidx);
        if(pcb->currentIndex() == 0 || pRoles->toPlainText().isEmpty() || pDetal->toPlainText().isEmpty() || pDetal->toPlainText().toInt() == 0){
            continue;
        }
        QString sval(pRoles->toPlainText().append(";").append(pcb->currentText()).append(";").append(pDetal->toPlainText()));
        option.attrsChanged.push_back(sval);
    }

    option.events.clear();
    try {
        auto qsl = ui->txt_events->toPlainText().split(";");
        for(auto &v:qsl){
            option.events.push_back(v.toInt());
        }
    } catch (...) {
        QMessageBox::information(nullptr,"错误","请检查事件输入项是否正确！！！");
    }
    MazeHelper::getInstance()->setOption(option);
    MazeHelper::getInstance()->save();
    close();
    MsgCenter::getInstance()->notify(key2str(MsgKeys::UpdateArrowText),MsgInt(currOptionId));
}


