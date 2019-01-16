#include "setbasicinfo.h"
#include "ui_setbasicinfo.h"
#include "formmgr.h"
#include "basejsonhelper.h"
#include "msgcenter.h"
#include "excelres.h"
#include <QVector>
#include <utility>

void loadItems(QVector<Item> &res)
{
    ExItem excel;
    excel.openExcel(EXCEL_FILES::item);
    excel.loadExcel();
    res = excel.getValidTickets();
    excel.closeExcel();
}

SetBasicInfo::SetBasicInfo(QWidget *parent) : QDialog(parent), ui(new Ui::SetBasicInfo)
  ,isCreateNew(true)
{
    ui->setupUi(this);
    MsgCenter::getInstance()->attach(key2str(MsgKeys::OpenSetBasicInfoForNew), [&](const std::string &key, const BaseMsg &msg) {
        this->acceptNotify(key,msg);
    });
    MsgCenter::getInstance()->attach(key2str(MsgKeys::OpenSetBasicInfoForSet), [&](const std::string &key, const BaseMsg &msg) {
        this->acceptNotify(key,msg);
    });
}

SetBasicInfo::~SetBasicInfo()
{
    MsgCenter::getInstance()->detach(key2str(MsgKeys::OpenSetBasicInfoForNew));
    MsgCenter::getInstance()->detach(key2str(MsgKeys::OpenSetBasicInfoForSet));
    delete ui;
}

void SetBasicInfo::initForNew()
{
    QVector<Item> items;
    loadItems(items);
    QStringList qsl({"none"});
    for(auto it = items.cbegin();it != items.cend();++it){
        QString s;
        s+= QString::number( it->id);
        s+= ":";
        s+= it->remark;
        qsl.append(s);
    }
    QVector<decltype (ui->comboBox)> comboxs{ ui->comboBox,ui->comboBox_2,ui->comboBox_3,ui->comboBox_4};
    for(auto cb : comboxs){
        cb->addItems(qsl);
        cb->setCurrentIndex(0);
    }

    ui->textEdit->setText("");
    ui->textEdit_2->setText("");
    ui->textEdit_3->setText("");
    ui->textEdit_4->setText("");

    ui->textEdit_id->setText("");
    ui->textEdit_name->setText("");
    ui->textEdit_potid->setText("");
    ui->textEdit_bgi->setText("");
}

void SetBasicInfo::initForSet()
{
    QVector<Item> items;
    loadItems(items);
    auto funcGetIdx = [&items](int itemId){
        int idx = -1;
        for(const auto &val: items){
            ++idx;
            if(val.id == itemId){
                return idx;
            }
        }
        return -1;
    };
    QStringList qsl({"none"});
    for(auto it = items.cbegin();it != items.cend();++it){
        QString s;
        s+= QString::number( it->id);
        s+= ":";
        s+= it->remark;
        qsl.append(s);
    }

    QVector<decltype (ui->comboBox)> comboxs{ ui->comboBox,ui->comboBox_2,ui->comboBox_3,ui->comboBox_4};
    for(auto cb : comboxs){
        cb->addItems(qsl);
    }
    QVector<decltype (ui->textEdit)> textEdits{ ui->textEdit, ui->textEdit_2, ui->textEdit_3, ui->textEdit_4};
    auto &mazeData = MazeHelper::getInstance()->getCurrMaze();
    auto keys = mazeData.tickets.keys();
    for(int index = 0;index !=4;++index){
        if(index < keys.size()){
            auto itemId = keys.at(index);
            auto idx = funcGetIdx(itemId) + 1;//因为第一位是none
            comboxs[index]->setCurrentIndex(idx == -1? 0 :idx);
            textEdits[index]->setText(QString::number(mazeData.tickets[itemId]));
        }else{
            comboxs[index]->setCurrentIndex(0);
            textEdits[index]->setText("");
        }
    }

    ui->textEdit_id->setText(QString::number(mazeData.id));
    ui->textEdit_name->setText(mazeData.name);
    ui->textEdit_potid->setText(QString::number(mazeData.potId));
    ui->textEdit_bgi->setText((mazeData.bgi));
}

void SetBasicInfo::acceptNotify(const std::string &key, const BaseMsg &msg)
{
    if(key == key2str(MsgKeys::OpenSetBasicInfoForNew))
    {
        isCreateNew = true;
        initForNew();
    }else if(key ==  key2str(MsgKeys::OpenSetBasicInfoForSet))
    {
        isCreateNew = false;
        initForSet();
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

    auto qsBgi = ui->textEdit_bgi->toPlainText();
    if (qsBgi.isEmpty())
    {
        tips.append("输入的背景是空的");
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

    MazeData maze;
    int originalId  = 0;
    if(!isCreateNew){
        maze = MazeHelper::getInstance()->copyCurrMaze();
        originalId = maze.id;
    }
    maze.id = qsId.toInt();
    maze.potId = qsPotId.toInt();
    maze.name = qsName;
    maze.tickets = tepTickets;
    maze.bgi = qsBgi;
    if(isCreateNew){
        auto result = MazeHelper::getInstance()->checkIsValid(maze);
        if(!std::get<0>(result)){
            pf(std::get<1>(result));
            return;
        }
        MazeHelper::getInstance()->create(maze);
        MazeHelper::getInstance()->setCurrMaze(maze.id);
    }else{
        auto result = MazeHelper::getInstance()->modifyIsValid(originalId,maze);
        if(!std::get<0>(result)){
            pf(std::get<1>(result));
            return;
        }
        MazeHelper::getInstance()->setCurrMaze(maze);
    }
    MazeHelper::getInstance()->save();

    MsgCloseTipsDlg msg;
    msg.status = isCreateNew;
    MsgCenter::getInstance()->notify(key2str(MsgKeys::ConfirmModifyBasicInfo), msg);

    FormMgr::getInstance()->close("setbasicinfo");
}

void SetBasicInfo::on_btn_cancel_clicked()
{
    FormMgr::getInstance()->close("setbasicinfo");
}
