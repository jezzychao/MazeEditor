#include "setbasicinfo.h"
#include "ui_setbasicinfo.h"
#include "formmgr.h"
#include "basejsonhelper.h"
#include "msgcenter.h"
#include "excelres.h"
#include <QVector>

SetBasicInfo::SetBasicInfo(QWidget *parent) : QDialog(parent), ui(new Ui::SetBasicInfo)
    ,data(nullptr),
    sp_exitem(new ExItem)
{
    ui->setupUi(this);
    MsgCenter::getInstance()->attach(key2str(MsgKeys::OpenSetBasicInfo), [&](const std::string &key, const BaseMsg &msg) {
        this->acceptNotify(key,msg);
    });
}

SetBasicInfo::~SetBasicInfo()
{
    MsgCenter::getInstance()->detach(key2str(MsgKeys::OpenSetBasicInfo));
    delete ui;
}

void SetBasicInfo::init(const BaseMsg &msg)
{
    auto &newMsg = static_cast<const MsgSetBasicInfo &>(msg);
    data.reset(new MsgSetBasicInfo(newMsg));
    sp_exitem->openExcel(EXCEL_FILES::item);
    sp_exitem->loadExcel();
    auto items = sp_exitem->getValidTickets();
    sp_exitem->closeExcel();
    sp_exitem.reset();

    QVector<int> allItemIds;

    QStringList qsl({"none"});
    for(auto it = items.cbegin();it != items.cend();++it){
        allItemIds.append(it->id);
        QString s;
        s+= QString::number( it->id);
        s+= ":";
        s+= it->remark;
        qsl.append(s);
    }
    ui->comboBox->addItems(qsl);
    ui->comboBox_2->addItems(qsl);
    ui->comboBox_3->addItems(qsl);
    ui->comboBox_4->addItems(qsl);

    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_3->setCurrentIndex(0);
    ui->comboBox_4->setCurrentIndex(0);

    if(!newMsg.isEmpty){
        ui->textEdit_id->setText(QString::number(newMsg.id));
        ui->textEdit_name->setText(newMsg.name);
        ui->textEdit_potid->setText(QString::number(newMsg.potId));
        int index = -1;
        for(auto it = newMsg.tickets.cbegin();it != newMsg.tickets.cend();++it)
        {
            ++index;
            if(index == 0)
            {
                auto idx = allItemIds.indexOf(it.value());
                if(idx != -1){
                    ui->comboBox->setCurrentIndex(idx);
                }
            }else if(index == 2)
            {
                auto idx = allItemIds.indexOf(it.value());
                if(idx != -1){
                    ui->comboBox_2->setCurrentIndex(idx);
                }
            }else if(index == 3)
            {
                auto idx = allItemIds.indexOf(it.value());
                if(idx != -1){
                    ui->comboBox_3->setCurrentIndex(idx);
                }
            }else if(index == 4)
            {
                auto idx = allItemIds.indexOf(it.value());
                if(idx != -1){
                    ui->comboBox_4->setCurrentIndex(idx);
                }
            }
        }

    }else{
        ui->textEdit_id->setText("");
        ui->textEdit_name->setText("");
        ui->textEdit_potid->setText("");
    }
}

void SetBasicInfo::acceptNotify(const std::string &key, const BaseMsg &msg)
{
    if(key == key2str(MsgKeys::OpenSetBasicInfo)){
        init(msg);
    }
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



    ExPot expot;
    expot.openExcel(EXCEL_FILES::pot);
    expot.loadExcel();
    auto &pot = static_cast<Pot&>(expot.at(qsPotId.toInt()));
    pot.stage.clear();
    pot.stage.append();
    expot.saveExcel();

    MsgSetBasicInfo msg;
    data->isEmpty = false;
    data->id = qsPotId.toInt();
    data->name = qsName;
    data->potId = qsPotId.toInt();

    //获取当前门票
    QMap<int,int> tepTickets;
    QVector<decltype (ui->comboBox)> comboxs{ ui->comboBox, ui->comboBox_2, ui->comboBox_3, ui->comboBox_4};
     QVector<decltype (ui->textEdit)> textEdits{ ui->textEdit, ui->textEdit_2, ui->textEdit_3, ui->textEdit_4};
     for(auto beg = 0; beg != 4;++beg){
         auto currText = comboxs[beg]->currentText();
         if(currText != "none"){
               auto itemId = currText.split(":")[0].toInt();
               QString snum = textEdits[beg]->toPlainText();
               if(!snum.isEmpty() && snum.toInt() > 0){
                   tepTickets.insert(itemId,snum.toInt());
               }
         }
     }
    data->tickets = tepTickets;

    MsgCenter::getInstance()->notify(key2str(MsgKeys::ConfirmModifyBasicInfo), msg);
    FormMgr::getInstance()->close("setbasicinfo");
}

void SetBasicInfo::on_btn_cancel_clicked()
{
    FormMgr::getInstance()->close("setbasicinfo");
}
