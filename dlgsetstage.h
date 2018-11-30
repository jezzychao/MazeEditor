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

    void on_typeCombox_indexChanged(int index);

private:
    void initForCrossing();

    void initForNpcFight();

    void initForCollect();

    void initForBarrier();

    void initForMinigame();

    void initForUploading();

    void initForPlayerFight();

    void disabledAllInput();

    ///@brief 检测后置路径是否重复
    std::tuple<bool,QString> checkForDuplication();

    Ui::DlgSetStage *ui;
    int stageId;
    QVector<int> allStageIds;
};

#endif // DLGSETSTAGE_H
