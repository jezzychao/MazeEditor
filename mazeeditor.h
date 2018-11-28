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
    void on_btn_save_clicked();

    void on_btn_new_clicked();

    void on_btn_open_clicked();

    void on_btn_set_clicked();

private:
    void acceptNotify(const std::string &, const BaseMsg &);

    ///@brief 清空可视化界面
    void clearView();

    ///@brief 根据数据来创建可视化界面
    void initView();

    Ui::mazeeditor *ui;
};

#endif // MAZEEDITOR_H
