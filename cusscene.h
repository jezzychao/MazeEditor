#ifndef CUSSCENE_H
#define CUSSCENE_H

#include <QGraphicsScene>
#include <QMap>
#include <memory>

class CusRect;
class CusArrow;

class CusScene:public QGraphicsScene
{
public:
    CusScene(QObject *parent = nullptr);
    ~CusScene() override;

    void reset();

    void addRect(const QPointF &);

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

    QMap<int, std::shared_ptr<CusRect>> allRects;
    QMap<int, std::shared_ptr<CusArrow>> allArrows;
};

#endif // CUSSCENE_H
