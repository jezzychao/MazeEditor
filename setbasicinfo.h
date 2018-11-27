#ifndef SETBASICINFO_H
#define SETBASICINFO_H

#include <QDialog>
#include <QMap>
#include <QString>
#include <memory>

namespace Ui {
class SetBasicInfo;
}

struct BaseMsg;

class ExItem;

struct MsgSetBasicInfo;

class SetBasicInfo : public QDialog
{
    Q_OBJECT

public:
    explicit SetBasicInfo(QWidget *parent = nullptr);
    ~SetBasicInfo();

private slots:

    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

private:
        void initForNew();

        void initForSet();


        void acceptNotify(const std::string &, const BaseMsg &);

private:
    Ui::SetBasicInfo *ui;
    bool isCreateNew;
};

#endif // SETBASICINFO_H
