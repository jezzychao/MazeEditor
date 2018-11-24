#ifndef TIPSDIALOG_H
#define TIPSDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui
{
class TipsDialog;
}

struct BaseMsg;

class TipsDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit TipsDialog(QWidget *parent = nullptr);
    ~TipsDialog();
  private slots:

    void on_btn_ok_clicked();

  private:
    void acceptNotify(const std::string &, const BaseMsg &);
    Ui::TipsDialog *ui;
};

#endif // TIPSDIALOG_H
