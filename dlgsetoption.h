#ifndef DLGSETOPTION_H
#define DLGSETOPTION_H

#include <QDialog>

namespace Ui {
class DlgSetOption;
}

class DlgSetOption : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSetOption(QWidget *parent = nullptr);
    ~DlgSetOption();

    void init(int optionId);

private slots:
    void on_pushButton_clicked();

private:
    Ui::DlgSetOption *ui;
    int currOptionId;
};

#endif // DLGSETOPTION_H
