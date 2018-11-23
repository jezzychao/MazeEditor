#include "setbasicinfo.h"
#include "ui_setbasicinfo.h"
#include  "formmgr.h"
#include "basejsonhelper.h"

SetBasicInfo::SetBasicInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetBasicInfo)
{
    ui->setupUi(this);
}

SetBasicInfo::~SetBasicInfo()
{
    delete ui;
}

void SetBasicInfo::on_pushButton_clicked()
{
    auto qsId = ui->textEdit_id->toPlainText();
    if(qsId.isEmpty()){
        //id是空
    }

    auto qsName = ui->textEdit_name->toPlainText();
    if(qsName.isEmpty()){
        //名字是空
    }

    auto qsPotId =ui->textEdit_potid->toPlainText();
    if(qsPotId.isEmpty()){
        //potid是空
    }

    MazePotData data(qsId.toInt(),qsName, qsPotId.toInt(), {});
    auto result = MazePotHelper::getInstance()->setMazePot(data);
    if(std::get<0>(result)){

    }
//    MazePotData data(,,);
//    MazePotHelper::getInstance()->setMazePot(,);

//TODO: 先检测配置信息是否正确，根据结果执行是否关闭
    FormMgr::getInstance()->close("setbasicinfo");
}
