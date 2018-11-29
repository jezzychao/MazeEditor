#ifndef DLGSETSTAGE_H
#define DLGSETSTAGE_H

#include <QDialog>

namespace Ui {
class DlgSetStage;
}

class BaseMsg;

class DlgSetStage : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSetStage(QWidget *parent = nullptr);
    ~DlgSetStage();

    void init(int stageId);

private slots:
    void on_pushButton_clicked();

private:

    Ui::DlgSetStage *ui;
    int stageId;
};

#endif // DLGSETSTAGE_H