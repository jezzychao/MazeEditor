#ifndef BASEJSONHELPER_H
#define BASEJSONHELPER_H

#include <QString>
#include <QJsonObject>
#include <memory>
#include <QMap>

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
    ///@brief 迷宫开始的stage id
    int beginStageId;
    ///@brief 进入迷宫所需要的门票
    QMap<int,int> tickets;
    ///@brief 迷宫的stage在编辑器中的显示位置
    QMap<int, MazeStagePos> stages;
};

class SingleMaze
{
public:
    SingleMaze(const MazeData &);

    const MazeData &get()const{return *sp;}

    bool setId(int);
    bool setName(const QString &);
    bool setBeginStageId(int );
    bool setTickets(const QMap<int,int>& );

    bool genStage(int,const  MazeStagePos &);
    bool delStage(int);

    ///@brief 若不存在，会创建；存在会修改
    bool setStage(int, const MazeStagePos &);

     ///@brief 自动生成Id
    int genStageId() const;

private:
    std::shared_ptr<MazeData> sp;
};

class MazeHelper: public BaseJSONHelper
{
public:
    static MazeHelper *getInstance();
    ~MazeHelper() override ;

protected:
    void read(const QJsonObject &) override;
    void write(QJsonObject &) override;
private:
    MazeHelper();
    QMap<int, SingleMaze> datamap;
};



#endif // BASEJSONHELPER_H
