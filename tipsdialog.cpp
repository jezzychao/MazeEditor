#include "tipsdialog.h"
#include "ui_tipsdialog.h"
#include "msgcenter.h"
#include "formmgr.h"

TipsDialog::TipsDialog(QWidget *parent)
    : QDialog(parent,Qt::WindowTitleHint | Qt::CustomizeWindowHint),
    ui(new Ui::TipsDialog)
{
    ui->setupUi(this);
    MsgCenter::getInstance()->attach(key2str(MsgKeys::OpenTipsDlg), [&](const std::string &key, const BaseMsg &msg) {
        this->acceptNotify(key,msg);
    });
}

TipsDialog::~TipsDialog()
{
    MsgCenter::getInstance()->detach(key2str(MsgKeys::OpenTipsDlg));
    delete ui;
}

void TipsDialog::on_btn_ok_clicked()
{
    FormMgr::getInstance()->close("tipsdialog");
    MsgCenter::getInstance()->notify(key2str(MsgKeys::CloseTipsDlg), MsgNull());
}

void TipsDialog::acceptNotify(const std::string &key, const BaseMsg &msg)
{
    auto &newMsg = static_cast<const MsgOpenTipsDlg &>(msg);
    ui->textBrowser->setText(newMsg.text.c_str());
}
