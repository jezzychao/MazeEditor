#ifndef DLGRUNTEST_H
#define DLGRUNTEST_H

#include <QDialog>
#include <memory>

struct MazeData;

namespace Ui {
class DlgRunTest;
}

class DlgRunTest : public QDialog
{
    Q_OBJECT

public:
    explicit DlgRunTest( std::shared_ptr<MazeData> ,QWidget *parent = nullptr);
    ~DlgRunTest();

private slots:

    void on_pbtn_0_clicked();

    void on_pbtn_1_clicked();

    void on_pbtn_2_clicked();

    void on_pushButton_clicked();

private:
    void refresh();
    void switch2next(int);
    void doClicked(int index);


    Ui::DlgRunTest *ui;
    std::shared_ptr<MazeData> sp_maze;
    int currStageId;
};

#endif // DLGRUNTEST_H
