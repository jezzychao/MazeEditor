#ifndef CUSSTAGE_H
#define CUSSTAGE_H

#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QVector>
#include <QMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <memory>
#include <QDrag>
#include <QGraphicsTextItem>

class CusArrow;
class BaseMsg;

class CusRect :public QGraphicsRectItem
{
//    Q_OBJECT
public:
    CusRect(int,QGraphicsItem *parent = nullptr);
    ~CusRect()override;
    bool attachArrow(std::shared_ptr<CusArrow>);
    void detachArrow(int);

    QPointF getCenterPos()const;

    int getId()const{return id;}

    void asEntry();

    void asExit();

    void updateText();

    QVector<std::shared_ptr<CusArrow>> getArrows(){return arrows;}

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
     void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
     void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    void changeArrows();
    void updateRectPosData();

    int id;//对应 mazestage 中的id
    QVector<std::shared_ptr<CusArrow>> arrows;//和rect相关联的所有箭头
    QGraphicsTextItem *text;
};

#endif // CUSSTAGE_H
