#ifndef DLGSETSTAGE_H
#define DLGSETSTAGE_H

#include <QDialog>
#include <QVector>
#include <utility>

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

    void on_pushButton_2_clicked();

private:
    std::tuple<bool,QString> inputIsValid();

    Ui::DlgSetStage *ui;
    int stageId;
    QVector<int> allStageIds;
};

#endif // DLGSETSTAGE_H
