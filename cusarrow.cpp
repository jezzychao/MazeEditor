#include "cusarrow.h"
#include "cusrect.h"
#include <QPainter>
#include "msgcenter.h"
#include "basejsonhelper.h"
#include <QMessageBox>

namespace  {
const double Pi = 3.14159265358979323846264338327950288419717;
const double TwoPi = 2.0 *Pi;
}

CusArrow::CusArrow(int f,std::shared_ptr<CusRect> start,std::shared_ptr<CusRect> end,QGraphicsItem *parent)
    :QGraphicsItem(parent),
      id(f),
      rectA(start),
      rectB(end),
      text(new QGraphicsTextItem(this))
{
    setZValue(0);
    adjust();
    text->setDefaultTextColor(Qt::black);  // 文本色
    updateText();
}

CusArrow::~CusArrow()
{

}

QRectF CusArrow::boundingRect() const
{
    qreal extra = (1.5 + 8) / 2;
    return QRectF(p1,QSizeF(p2.x() - p1.x(),p2.y() - p1.y())).normalized().adjusted(-extra,-extra,extra,extra);
}

void CusArrow::adjust()
{
    prepareGeometryChange();
    QPointF pt = mapFromItem(rectB.get(),rectB->rect().width() / 2,rectB->rect().height() / 2);
    QPointF ps = mapFromItem(rectA.get(),rectA->rect().width() / 2,rectA->rect().height() / 2);
    p1 = ps;
    if(ps.y() < pt.y()){
        p2 = QPointF(pt.x(),pt.y()-rectB->rect().height() / 2);
    }else{
        p2 = QPointF(pt.x(),pt.y()+rectB->rect().height() / 2);
    }

    QLineF line(p1,p2);
    double angle = ::acos(line.dx() / line.length());
    if(line.dy() >= 0){
        angle = TwoPi - angle;
    }

    arrowP1 = p2 + QPointF(sin(angle- Pi / 3)*8,cos(angle - Pi / 3)*8);
    arrowP2 = p2 + QPointF(sin(angle - Pi + Pi / 3)*8,cos(angle - Pi + Pi / 3) *8);
    updateTextItemPos();
}

void CusArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *style,QWidget *w)
{
    painter->setRenderHint(QPainter::Antialiasing);
    if(isCanEditInfo()){
        painter->setBrush(QBrush(QColor::fromRgb(79,136,187)));
        QPen p(QColor::fromRgb(79,136,187));
        p.setWidthF(1.5);
        painter->setPen(p);
    }else{
        painter->setBrush(QBrush(Qt::gray));
        QPen p(Qt::gray);
        p.setWidthF(1.5);
        painter->setPen(p);
    }
    painter->drawLine(p1,p2);
    painter->drawPolygon(QPolygonF()<<p2<<arrowP1<<arrowP2);
}

void CusArrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(isCanEditInfo()){
        MsgCenter::getInstance()->notify(key2str(MsgKeys::OpenDlgSetOption),MsgInt(id));
    }else{
        QMessageBox::information(nullptr,"信息","该箭头不能进行编辑！");
    }
}

void CusArrow::updateText()
{
    auto currOption = MazeHelper::getInstance()->getOption(getId());
    QString showTxt;
    showTxt += currOption.remark;
    text->setPlainText(showTxt);
}

void CusArrow::updateTextItemPos()
{
    auto centerPos = (p2+p1) / 2;
    text->setPos(centerPos);
}

bool CusArrow::isCanEditInfo()
{
    auto stageId = rectA->getId();
    auto stage = MazeHelper::getInstance()->getStage(stageId);
    return stage.type == 1;
}

