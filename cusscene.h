#ifndef CUSSCENE_H
#define CUSSCENE_H

#include <QGraphicsScene>
#include <QMap>
#include <memory>

class CusRect;
class CusArrow;
class BaseMsg;

class CusScene:public QGraphicsScene
{
public:
    CusScene(QObject *parent = nullptr);
    ~CusScene() override;

    void reset();

    void addRect(const QPointF &);

    void delRect(int rectId);

protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    //    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    //    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    //    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    //    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private slots:
    void on_create_rect(bool);

private :
    void ctor();
    void dtor();

    void acceptNotify(const std::string &, const BaseMsg &);

    void resetNextArrows(std::shared_ptr<CusRect>);
    void delArrows(std::shared_ptr<CusRect>);

    QMap<int, std::shared_ptr<CusRect>> allRects;
    QMap<int, std::shared_ptr<CusArrow>> allArrows;

    int currSelectedRect;
    int currSelectedArrow;
};

#endif // CUSSCENE_H
