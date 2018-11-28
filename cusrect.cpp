#include "cusrect.h"
#include "cusarrow.h"

CusRect::CusRect(int f,QGraphicsItem *parent)
    :QGraphicsRectItem(parent),
      flag(f),
      arrows()
{

}

CusRect::~CusRect()
{
}

QVariant CusRect::itemChange(QGraphicsItem::GraphicsItemChange change,const QVariant &value)
{
    switch(change){
    case QGraphicsItem::GraphicsItemChange::ItemPositionHasChanged:
        for(auto &p : arrows){
            p->adjust();
        }
        break;
    default:
        break;
    }
    return QGraphicsItem::itemChange(change,value);
}

bool CusRect::attachArrow(std::shared_ptr<CusArrow> sp)
{
    for(const auto &p: arrows){
        if(p->getFlag() == sp->getFlag()){
            qWarning("Already exist arrow:%d in rect:%d.",sp->getFlag(),flag);
            return false;
        }
    }
    arrows.push_back(sp);
    return true;
}

void CusRect::detachArrow(int arrowflag)
{
    for(auto it = arrows.begin();it !=arrows.end();++it){
        if((*it)->getFlag() == arrowflag){
            arrows.erase(it);
            return;
        }
    }
}
