#ifndef CUSSTAGE_H
#define CUSSTAGE_H

#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QVector>
#include <memory>

class CusArrow;

class CusRect :public QGraphicsRectItem
{
//    Q_OBJECT
public:
    CusRect(int,QGraphicsItem *parent = nullptr);
    ~CusRect()override;
    bool attachArrow(std::shared_ptr<CusArrow>);
    void detachArrow(int);

    int getFlag()const{return flag;}
protected:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange,const QVariant &) override;
private:
    int flag;//对应 mazestage 中的id
    QVector<std::shared_ptr<CusArrow>> arrows;//和rect相关联的所有箭头

};

#endif // CUSSTAGE_H
