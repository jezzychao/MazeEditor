#ifndef DLGCONFIRM_H
#define DLGCONFIRM_H

#include <QDialog>
#include <QString>

namespace Ui {
class DlgConfirm;
}

class DlgConfirm : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConfirm(const QString &,QWidget *parent = nullptr);
    ~DlgConfirm();

private:
    Ui::DlgConfirm *ui;
};

#endif // DLGCONFIRM_H
