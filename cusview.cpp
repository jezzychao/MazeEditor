#include "cusview.h"
#include "cusscene.h"

CusView::CusView(QWidget *parent )
    :QGraphicsView(parent),
      scene(new CusScene),
      menu(new QMenu),
      addAct(new QAction("创建 Stage", this))
{
    setScene(scene);
    setDragMode(QGraphicsView::ScrollHandDrag);
    menu->addAction(addAct);
    connect(addAct, SIGNAL(triggered(bool)), this, SLOT(on_create_rect(bool)));
}

CusView::~CusView()
{
    delete scene;
    delete menu;
    delete addAct;
}

void CusView::reset()
{
    scene->reset();
}

void CusView::on_create_rect(bool)
{
    scene->addRect(QPointF(0,0));
}

void CusView::mousePressEvent(QMouseEvent *event)
{
    qDebug("*********CusView::mousePressEvent***************");
    if(event->button() == Qt::RightButton)
    {
        qWarning("trigger right button.");
        menu->exec(event->globalPos());
    }
    QGraphicsView::mousePressEvent(event);
}
