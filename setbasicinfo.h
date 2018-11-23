#ifndef SETBASICINFO_H
#define SETBASICINFO_H

#include <QDialog>

namespace Ui {
class SetBasicInfo;
}

class SetBasicInfo : public QDialog
{
    Q_OBJECT

public:
    explicit SetBasicInfo(QWidget *parent = nullptr);
    ~SetBasicInfo();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SetBasicInfo *ui;
};

#endif // SETBASICINFO_H
