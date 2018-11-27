#ifndef DLGOPENMAZE_H
#define DLGOPENMAZE_H

#include <QDialog>

namespace Ui {
class DlgOpenMaze;
}

class DlgOpenMaze : public QDialog
{
    Q_OBJECT

public:
    explicit DlgOpenMaze(QWidget *parent = nullptr);
    ~DlgOpenMaze();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DlgOpenMaze *ui;
};

#endif // DLGOPENMAZE_H
