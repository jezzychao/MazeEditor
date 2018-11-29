#include "cusrect.h"
#include "cusarrow.h"
#include <QPainter>
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>
#include "basejsonhelper.h"
#include "formmgr.h"
#include "msgcenter.h"
#include "dlgsetstage.h"
#include <QMessageBox>

CusRect::CusRect(int f,QGraphicsItem *parent)
    :QGraphicsRectItem(QRectF(0,0,100,66),parent),
      id(f),
      arrows()
{
    setZValue(10);
    //    setFlag(QGraphicsItem::ItemIsMovable,true);
    setAcceptDrops(true);
    setBrush(QBrush(QColor::fromRgb(0,160,230)));
}

CusRect::~CusRect()
{
}

QPointF CusRect::getCenterPos()const
{
    return pos() +  QPointF(rect().width() / 2,rect().height() / 2);
}

bool CusRect::attachArrow(std::shared_ptr<CusArrow> sp)
{
    for(const auto &p: arrows){
        if(p->getId() == sp->getId()){
            qWarning("Already exist arrow:%d in rect:%d.",sp->getId(),id);
            return false;
        }
    }
    arrows.push_back(sp);
    return true;
}

void CusRect::detachArrow(int arrowflag)
{
    for(auto it = arrows.begin();it !=arrows.end();++it){
        if((*it)->getId() == arrowflag){
            arrows.erase(it);
            return;
        }
    }
}

void CusRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        qWarning("mousePressEvent");
        auto mimeData = new QMimeData;
        mimeData->setColorData(QVariant(QColor(Qt::green)));
        auto  drag = new QDrag(event->widget());
        drag->setMimeData(mimeData);
        drag->start();
    }
}

void CusRect::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    qWarning("cusrect mouseDoubleClickEvent");
    if(getId() == 0){
        QMessageBox::information(nullptr,QString("警告"),QString("不可对出口标记进行编辑"));
        return;
    }
    DlgSetStage *dlg = new DlgSetStage();
    dlg->init(id);
    dlg->show();
}

void CusRect::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    qWarning("dragEnterEvent");
}

void CusRect::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    qWarning("dragMoveEvent");
    setPos( event->pos() + pos() - QPointF(this->rect().width() / 2, this->rect().height() / 2));
    changeArrows();
}

void CusRect::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    qWarning("dragLeaveEvent");
    updateRectPosData();
}

void CusRect::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    qWarning("dropEvent");
    setPos( event->pos() + pos() - QPointF(this->rect().width() / 2, this->rect().height() / 2));
    changeArrows();
    updateRectPosData();
}

void CusRect::changeArrows()
{
    for(auto arr :arrows){
        arr->adjust();
    }
}

void CusRect::asEntry()
{
    setBrush(QBrush(Qt::green));
}

void CusRect::asExit()
{
    setBrush(QBrush(Qt::red));
}

void CusRect::updateRectPosData()
{
    auto stage = MazeHelper::getInstance()->getStage(id);
    stage.pos = pos().toPoint();
    MazeHelper::getInstance()->setStage(stage);
}
