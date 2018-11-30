#include "dlgsetoption.h"
#include "ui_dlgsetoption.h"
#include "basejsonhelper.h"
#include "msgcenter.h"
#include <QMessageBox>

DlgSetOption::DlgSetOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSetOption),
    currOptionId(0)
{
    ui->setupUi(this);
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


