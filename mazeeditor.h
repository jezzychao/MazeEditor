#ifndef MAZEEDITOR_H
#define MAZEEDITOR_H

#include <QMainWindow>
#include <memory>

namespace Ui {
class mazeeditor;
}

struct BaseMsg;

class mazeeditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit mazeeditor(QWidget *parent = nullptr);
    ~mazeeditor();

private slots:
    void on_actionview_triggered();

    void on_actionedit_triggered();

    void on_actionnew_triggered();

    void on_btn_basicInfo_clicked();

private:
    void acceptNotify(const std::string &, const BaseMsg &);

    Ui::mazeeditor *ui;
};

#endif // MAZEEDITOR_H
