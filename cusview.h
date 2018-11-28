#ifndef CUSVIEW_H
#define CUSVIEW_H

#include <QScrollArea>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>


class CusView : public QScrollArea
{
    Q_OBJECT
public:
    CusView(QWidget *pw = nullptr);
    ~CusView();

    ///@brief 创建视图区域
    void initView();

    ///@brief 清空视图区域
    void clearView();

protected:
    void mousePressEvent(QMouseEvent *);
    //    void mouseReleaseEvent(QMouseEvent *);
    //    void mouseDoubleClickEvent(QMouseEvent *);
    //    void mouseMoveEvent(QMouseEvent *);
//    void wheelEvent(QWheelEvent *);

    private slots:
        void on_rightbutton_triggered(bool);

private:
    ///@brief 创建右键菜单
    void initRBMenu();

    ///@brief 创建一个方块
    void createStage();

    ///@brief 删除一个方块，同时清除入度和出度
    //void deleteStage();



private:
    QMenu *m_pMenu;
    QAction *m_pAddAct;
};

#endif // CUSVIEW_H
