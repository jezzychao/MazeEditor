#include "cusview.h"
#include "basejsonhelper.h"
#include "cusrect.h"
#include "cusarrow.h"

CusView::CusView(QWidget * p)
    :QGraphicsView (p),
      m_pScene(new QGraphicsScene(this)),
      m_pMenu(nullptr),
      m_pAddAct(nullptr)
{
    m_pScene->setSceneRect(-2000,-2000,4000,4000);
    this->setScene(m_pScene);
    //    this->setBackgroundBrush(QBrush(QColor::fromRgb(79,136,187)));
    initRBMenu();

//    auto r1 = new CusRect(1);
//    auto r2 = new CusRect(2);
//    this->scene()->addItem(r1);
//    this->scene()->addItem(r2);
//    r1->setPos(QPointF(0,0));
//    r2->setPos(QPointF(200,200));
//    r1->show();
//    r2->show();


//    auto arrow = new CusArrow(1,std::shared_ptr<CusRect>(r1),std::shared_ptr<CusRect>(r2));
//    this->scene()->addItem(arrow);
//    arrow->show();

}

CusView::~CusView()
{
    delete m_pScene;
    delete m_pMenu;
    delete m_pAddAct;
}

void CusView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        qWarning("trigger right button.");
        m_pMenu->exec(event->globalPos());
    }
}

void CusView::initRBMenu()
{
    m_pMenu = new QMenu;
    m_pAddAct = new QAction("创建 Stage", this);
    m_pMenu->addAction(m_pAddAct);
    connect(m_pAddAct, SIGNAL(triggered(bool)), this, SLOT(on_rightbutton_triggered(bool)));
}

void CusView::on_rightbutton_triggered(bool)
{
    createStage();
}

void CusView::initView()
{
    qWarning("CusView initView.");
    //    auto &currMaze = MazeHelper::getInstance()->getCurrMaze();
    //    auto tempStage = this->findChild<CusStage *>("template_stage");
    //   CusStage * newStage = new CusStage(this);
    //   this->setWidget(newStage);
    //   newStage->update(340,70, newStage->geometry().width(),newStage->geometry().height());
    //newStage->show();
}

void CusView::clearView()
{
    qWarning("CusView clearView.");
}

void CusView::createStage()
{
    qWarning("CusView createStage.");

}
