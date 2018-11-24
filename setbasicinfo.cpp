#include "setbasicinfo.h"
#include "ui_setbasicinfo.h"
#include "formmgr.h"
#include "basejsonhelper.h"
#include "msgcenter.h"

SetBasicInfo::SetBasicInfo(QWidget *parent) : QDialog(parent), ui(new Ui::SetBasicInfo)
{
    ui->setupUi(this);
}

SetBasicInfo::~SetBasicInfo()
{
    delete ui;
}

void SetBasicInfo::on_btn_ok_clicked()
{
    QString tips;
    auto qsId = ui->textEdit_id->toPlainText();
    if (qsId.isEmpty())
    {
        //id是空
        tips.append("输入的迷宫ID是空的\n");
    }
    auto qsName = ui->textEdit_name->toPlainText();
    if (qsName.isEmpty())
    {
        //名字是空
        tips.append("输入的迷宫名称是空的\n");
    }
    auto qsPotId = ui->textEdit_potid->toPlainText();
    if (qsPotId.isEmpty())
    {
        //potid是空
        tips.append("输入的迷宫所在potId是空的");
    }

    //打开提示界面
    auto pf = [](const QString &qs){
        FormMgr::getInstance()->open("tipsdialog");
        MsgOpenTipsDlg msg;
        msg.text = qs.toStdString();
        MsgCenter::getInstance()->notify(key2str(MsgKeys::OpenTipsDlg), msg);
    };

    if (!tips.isEmpty())
    {
        pf(tips);
        return;
    }

    MazePotData data(qsId.toInt(), qsName, qsPotId.toInt(), {});
    MazePotHelper helper;
    helper.load();
     auto result = helper.setMazePot(data);
     if(!std::get<0>(result)){
         pf(std::get<1>(result));
         return;
     }
     helper.save();
    FormMgr::getInstance()->close("setbasicinfo");
}

void SetBasicInfo::on_btn_cancel_clicked()
{
    FormMgr::getInstance()->close("setbasicinfo");
}
