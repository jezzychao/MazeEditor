#ifndef CUSVIEW_H
#define CUSVIEW_H

#include <QGraphicsView>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QMouseEvent>

class CusScene;

class CusView:public QGraphicsView
{
        Q_OBJECT
public:
    CusView(QWidget *parent = nullptr);
    ~CusView()override;

    void reset();
    void clear();
protected:
    void mousePressEvent(QMouseEvent *) override;
private slots:
    void on_create_rect(bool);
   private:
    CusScene *scene;
    QMenu *menu;
    QAction *addAct;
};

#endif // CUSVIEW_H
