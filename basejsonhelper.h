#ifndef BASEJSONHELPER_H
#define BASEJSONHELPER_H

#include <QString>
#include <QJsonObject>
#include <memory>
#include <QMap>
#include <utility>
#include <QVector>
#include <QPoint>
#include <initializer_list>

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
    int id = 0;
    QString text = "";
    QString activecond = "";
    QString disabledTips = "";
    int linkStageId = -1;//如果是出口，该值为0
    QVector<int> events;
    bool isonlyonce = false;
    QStringList attrsChanged;

    QString remark;
};

struct MazeStage
{
    int id = 0;
    int type = 0;
    int mappingId = 0;
    QString name = "";
    QString sprite = "";
    QString desc = "";
    int entryStoryId = 0;

    QString remark;

    QMap<int,MazeOption> options;
    QPoint pos;//stage在view中的位置
    QVector<int> frontStageIds;//与该stage相连的前置stage
    /// 没有使用
    QVector<int> nextStageIds;//与该stage相连的后置stage
};

///单个迷宫在编辑器中的配置数据
struct MazeData{
    ///@brief id
    int id = 0;
    ///@brief 迷宫名称
    QString name = "";
    ///@brief 迷宫所在的怕potid
    int potId = 0;
    /// 没有使用
    int beginStageId = 0;
    /// 没有使用
    int endStageId = 0;
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

    void deleteMaze(int mazeId);

    ///@brief 获取所有迷宫的简要信息：名称和id
    QMap<int,QString> getBriefInfo() const;

    ///@brief 检测mazeid,potid,name,是否有效
    std::tuple<bool,QString> checkIsValid(const MazeData &);

    ///@brief 检测mazeid,potid,name的修改,是否有效
    std::tuple<bool,QString> modifyIsValid(int , const MazeData &);

    std::tuple<bool,QString> checkStageIsValid(const MazeStage &);

    std::tuple<bool,QString> checkOptionIsValid(int, const MazeOption &);

    MazeData copyCurrMaze()const ;

    ///@brief 获取所有stage的名称和id
    QMap<int,QString> getStageInfos(std::initializer_list<int> excludeIds = {})const;
    const MazeData &getCurrMaze()const;
    std::shared_ptr<MazeData>  getSpMaze(){
        return m_maze[currId];
    }
    bool setCurrMaze(const MazeData &);
    bool setCurrMaze(int);
    bool isAlreadyExist();


    ///@brief 生成最大的stageid
    int genNewStageId() const;
    ///@brief 会对该stage重新设置关联stage
    void setStage(MazeStage&);
    MazeStage getStage(int stageId);

    int genNewOptionId(const MazeStage &stage)const;
    void setOption(MazeOption &);
    MazeOption getOption(int optionId);

protected:
    void read(const QJsonObject &) override;
    void write(QJsonObject &) override;

    void readStage(MazeStage &,const QJsonObject &) const ;
    void writeStage(const MazeStage &,QJsonObject &)    ;

    void readOption(MazeOption &,const QJsonObject &)const;
    void writeOption(const MazeOption &,QJsonObject &);

private:
    MazeHelper();

    ///@brief 根据stageid查找关联前置的stageId
    QVector<int> findFrontStageIds(int stageId);
    ///@brief 根据stageid查找关联后置的stageId
    QVector<int> findNextStageIds(int stageId);

    QMap<int, std::shared_ptr<MazeData>> m_maze;
    int currId;
};

#endif // BASEJSONHELPER_H
