#include "cusscene.h"
#include "cusrect.h"
#include "cusarrow.h"
#include "basejsonhelper.h"
#include "msgcenter.h"

CusScene::CusScene(QObject *parent )
    :QGraphicsScene(QRectF(-2000,-2000,4000,4000),parent),
      allRects(),
      allArrows()
{
    MsgCenter::getInstance()->attach(key2str(MsgKeys::ResetNextArrows), [&](const std::string &key, const BaseMsg &msg) {
        this->acceptNotify(key,msg);
    });
}

CusScene::~CusScene()
{
    MsgCenter::getInstance()->detach(key2str(MsgKeys::ResetNextArrows));
}

void CusScene::acceptNotify(const std::string &key, const BaseMsg &msg)
{
    if(key == key2str(MsgKeys::ResetNextArrows)){
        const auto &data = static_cast<const MsgResetArrows&>(msg);

        for(auto id: data.delOpIds){
            auto it = allArrows.find(id);
            if(it != allArrows.end()){
                removeItem(it.value().get());
                allArrows.erase(it);
            }
        }

        for(auto &rt: allRects){
            for(auto arrowId: data.delOpIds){
                rt->detachArrow(arrowId);
            }
        }

        auto &rect = allRects[data.stageId];
        for(auto it = data.opIdLinkToStageId.cbegin(); it != data.opIdLinkToStageId.cend();++it){
            auto it_rect = allRects.find(it.value());
            if(it_rect != allRects.end()){
                auto arrow = std::shared_ptr<CusArrow>(new CusArrow(it.key(),rect, it_rect.value()));
                addItem(arrow.get());
                allArrows.insert(it.key(),arrow);
                rect->attachArrow(arrow);
                it_rect.value()->attachArrow(arrow);
            }
        }
    }
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
            if(maze.beginStageId == stage.id){
                it.value()->asEntry();
            }else if(maze.endStageId == stage.id){
                it.value()->asExit();
            }
        }

        //创建 arrow
        QVector<QPoint> used;//已经创建过的连线,x:起始stageid,y:终点stageid
        for(const auto &stage: maze.stages){
            auto it_startRect = allRects.find(stage.id);
            for(const auto &option: stage.options){
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
                    addItem(sp.get());
                    allArrows.insert(option.id,sp);
                    it_startRect.value()->attachArrow(sp);
                    it_endRect.value()->attachArrow(sp);
                    used.push_back(point);
                }
            }
        }
    }else{
        //创建出口
        MazeStage entryStage;
        entryStage.id = MazeHelper::getInstance()->genNewStageId();
        entryStage.remark = "入口";
        entryStage.pos = QPoint(0,-200);
        MazeHelper::getInstance()->setStage(entryStage);

        MazeStage exitStage;
        exitStage.id = 0;
        exitStage.remark = "出口";
        exitStage.pos = QPoint(0,200);
        MazeHelper::getInstance()->setStage(exitStage);

        auto copy = MazeHelper::getInstance()->copyCurrMaze();
        copy.beginStageId = entryStage.id;
        copy.endStageId = exitStage.id;
        MazeHelper::getInstance()->setCurrMaze(copy);

        MazeHelper::getInstance()->save();

        auto exit = std::shared_ptr<CusRect>(new CusRect(exitStage.id));
        allRects.insert(exitStage.id,exit);
        addItem(exit.get());
        exit->setPos(0,200);
        exit->asExit();

        auto entry = std::shared_ptr<CusRect>(new CusRect(entryStage.id));
        allRects.insert(entryStage.id,entry);
        addItem(entry.get());
        entry->setPos(0,-200);
        entry->asEntry();

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

void CusScene::resetNextArrows(std::shared_ptr<CusRect> rect)
{



}

void CusScene::delArrows(std::shared_ptr<CusRect> rect)
{

}
