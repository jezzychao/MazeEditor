#include "dlgopenmaze.h"
#include "ui_dlgopenmaze.h"
#include "basejsonhelper.h"
#include "msgcenter.h"
#include "formmgr.h"

DlgOpenMaze::DlgOpenMaze(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgOpenMaze)
{
    ui->setupUi(this);

    QStringList qls({"none"});
    auto info = MazeHelper::getInstance()->getBriefInfo();
    for(auto it = info.cbegin();it != info.cend();++it){
        qls.append(QString::number(it.key()) + ":" + it.value());
    }
    ui->comboBox->addItems(qls);
    ui->comboBox->setCurrentIndex(0);
}

DlgOpenMaze::~DlgOpenMaze()
{
    delete ui;
}

void DlgOpenMaze::on_pushButton_clicked()
{
    auto text = ui->comboBox->currentText();
    if(text != "none"){
        auto mazeId = text.split(":")[0].toInt();
        MazeHelper::getInstance()->setCurrMaze(mazeId);
        MazeHelper::getInstance()->save();
        MsgCenter::getInstance()->notify(key2str(MsgKeys::ConfirmOpenMaze), MsgNull());
    }
    FormMgr::getInstance()->close("dlgopenmaze");

}
