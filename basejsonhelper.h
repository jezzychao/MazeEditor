#ifndef BASEJSONHELPER_H
#define BASEJSONHELPER_H

#include <QString>
#include <QJsonObject>
#include <memory>
#include <QMap>
#include <utility>
#include <QVector>
#include <QPoint>

class BaseJSONHelper
{
public:
    BaseJSONHelper(const QString &);
    BaseJSONHelper(const BaseJSONHelper &) = delete;
    BaseJSONHelper &operator=(const BaseJSONHelper &) = delete;
    virtual ~BaseJSONHelper();
    virtual bool load();
    virtual bool save();

protected:
    virtual void read(const QJsonObject &) = 0;
    virtual void write(QJsonObject &) = 0;
    QString filename;
};

struct MazeOption
{
        int id;
        QString text;
        QString activecond;
        QString disabledTips;
        int linkStageId;//如果是出口，该值为0
        QVector<int> events;
        bool isonlyonce;
};

struct MazeStage
{
    int id;
    int type;
    int mappingId;
    QString name;
    QString sprite;
    QString desc;
    int entryStoryId;

    QMap<int,MazeOption> options;
    QPoint pos;//stage在view中的位置
    QVector<int> frontStageIds;//与该stage相连的前置stage
    QVector<int> nextStageIds;//与该stage相连的后置stage
};

///在编辑器中位置
struct MazeStagePos
{
    MazeStagePos(int _x = 0,int _y = 0):x(_x),y(_y){}
    int x;
    int y;
};

///单个迷宫在编辑器中的配置数据
struct MazeData{
    ///@brief id
    int id;
    ///@brief 迷宫名称
    QString name;
    ///@brief 迷宫所在的怕potid
    int potId;
    ///@brief 迷宫开始的stage id
    int beginStageId;
    ///@brief 进入迷宫所需要的门票
    QMap<int,int> tickets;

    ///@brief 迷宫的stage在编辑器中的显示位置
    QMap<int, MazeStage> stages;
};

class MazeHelper: public BaseJSONHelper
{
public:
    static MazeHelper *getInstance();
    ~MazeHelper() override ;

    ///@brief 尝试创建一个迷宫数据
    void create(const MazeData&);

    ///@brief 检测mazeid,potid,name,是否有效
    std::tuple<bool,QString> checkIsValid(const MazeData &);

    ///@brief 检测mazeid,potid,name的修改,是否有效
    std::tuple<bool,QString> modifyIsValid(int , const MazeData &);

    MazeData copyCurrMaze()const ;

    const MazeData &getCurrMaze()const;
    bool setCurrMaze(const MazeData &);
    bool setCurrMaze(int);
    bool isAlreadyExist();
    ///@brief 获取所有迷宫的简要信息：名称和id
    QMap<int,QString> getBriefInfo() const;
protected:
    void read(const QJsonObject &) override;
    void write(QJsonObject &) override;

    void readStage(MazeStage &,const QJsonObject &) const ;
    void writeStage(const MazeStage &,QJsonObject &)    ;

    void readOption(MazeOption &,const QJsonObject &)const;
    void writeOption(const MazeOption &,QJsonObject &);

private:
    MazeHelper();

    ///@brief 生成最大的stageid
    int genNewStageId(int) const;

    QMap<int, std::shared_ptr<MazeData>> m_maze;
    int currId;
};



#endif // BASEJSONHELPER_H
