#include "cusview.h"
#include "basejsonhelper.h"
#include "cusstage.h"

CusView::CusView(QWidget * p)
    :QScrollArea (p),
      m_pMenu(nullptr),
      m_pAddAct(nullptr)
{
    auto tempStage = this->findChild<CusStage *>("template_stage");
    this->setWidget(tempStage);
    initRBMenu();
}

CusView::~CusView()
{
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
    auto &currMaze = MazeHelper::getInstance()->getCurrMaze();
    auto tempStage = this->findChild<CusStage *>("template_stage");
   CusStage * newStage = new CusStage(this);
   this->setWidget(newStage);
   newStage->update(340,70, newStage->geometry().width(),newStage->geometry().height());
newStage->show();
}

void CusView::clearView()
{
    qWarning("CusView clearView.");
}

void CusView::createStage()
{
    qWarning("CusView createStage.");

}
