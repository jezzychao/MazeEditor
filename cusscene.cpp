#include "cusscene.h"
#include "cusrect.h"
#include "cusarrow.h"
#include "basejsonhelper.h"

CusScene::CusScene(QObject *parent )
    :QGraphicsScene(QRectF(-2000,-2000,4000,4000),parent),
      allRects(),
      allArrows()
{
}

CusScene::~CusScene()
{
}

void CusScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("*********MyScene::mousePressEvent***************");
    QGraphicsScene::mousePressEvent(event);
}

void CusScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("*********MyScene::mouseDoubleClickEvent***************");
    QGraphicsScene::mouseDoubleClickEvent(event);
}

void CusScene::reset()
{
    if(!MazeHelper::getInstance()->isAlreadyExist()){
        return;
    }
    auto &maze = MazeHelper::getInstance()->getCurrMaze();
    qDebug("reset scene by maze id: %d", maze.id);
    dtor();
    ctor();
}

void CusScene::ctor()
{
    auto &maze = MazeHelper::getInstance()->getCurrMaze();
    if(maze.stages.size() > 0)
    {
        //创建 rect
        for(const auto &stage: maze.stages){
            auto it =allRects.insert(stage.id,std::shared_ptr<CusRect>(new CusRect(stage.id)));
            it.value()->setPos(stage.pos);
            addItem(it.value().get());
            if(-1 == stage.id){
                it.value()->asEntry();
            }else if(0 == stage.id){
                it.value()->asExit();
            }
        }

        //创建 arrow
        QVector<QPoint> used;//已经创建过的连线,x:起始stageid,y:终点stageid
        for(const auto &stage: maze.stages){
            auto it_startRect = allRects.find(stage.id);
            for(const auto &option: stage.options){
                if(option.linkStageId != 0){//为0指的是退出选项
                    QPoint point(stage.id,option.linkStageId);
                    //查找是否已经创建过了
                    bool isExisted = false;
                    for(auto it_used = used.cbegin();it_used != used.cend();++it_used){
                        if(*it_used == point){
                            isExisted = true;
                            break;
                        }
                    }
                    if(isExisted){
                        break;
                    }
                    auto it_endRect = allRects.find(option.linkStageId);
                    if(it_endRect != allRects.end()){
                        auto sp = std::shared_ptr<CusArrow>(new CusArrow(option.id,it_startRect.value(),it_endRect.value()));
                        allArrows.insert(option.id,sp);
                        it_startRect.value()->attachArrow(sp);
                        it_endRect.value()->attachArrow(sp);
                        used.push_back(point);
                    }
                }
            }
        }
    }else{
        //创建出口
        MazeStage exit_data;
        exit_data.id = 0;
        exit_data.name = "exit";
        exit_data.pos = QPoint(0,200);
        MazeHelper::getInstance()->setStage(exit_data);

        MazeStage entry_data;
        entry_data.id = -1;
        entry_data.name = "entry";
         entry_data.pos = QPoint(0,-200);
        MazeHelper::getInstance()->setStage(entry_data);

        MazeHelper::getInstance()->save();

        auto exit = std::shared_ptr<CusRect>(new CusRect(0));
        auto entry = std::shared_ptr<CusRect>(new CusRect(-1));
        allRects.insert(0,exit);
        allRects.insert(-1,entry);
        addItem(exit.get());
        addItem(entry.get());
        entry->setPos(0,-200);
        entry->asEntry();
        exit->setPos(0,200);
        exit->asExit();
    }
}

void CusScene::dtor()
{
    for(auto v: allArrows){
        removeItem(v.get());
    }
    for(auto v: allRects){
        removeItem(v.get());
    }
    allArrows.clear();
    allRects.clear();
}

void CusScene::addRect(const QPointF &p)
{
    MazeStage stage;
    stage.id = MazeHelper::getInstance()->genNewStageId();
     MazeHelper::getInstance()->setStage(stage);
    auto exit = std::shared_ptr<CusRect>(new CusRect(stage.id));
    allRects.insert(stage.id,exit);
    addItem(exit.get());
    exit->setPos(p);

}
