#include "basejsonhelper.h"
#include "globalfunc.h"
#include <QStringLiteral>
#include <stdexcept>
#include <QJsonDocument>
#include <QtDebug>
#include <QByteArray>
#include <QTextStream>
#include <QFile>

BaseJSONHelper::BaseJSONHelper(const QString &file)
    : filename(file)
{
}

BaseJSONHelper::~BaseJSONHelper()
{
}

bool BaseJSONHelper::load()
{
    QFile loadFile(myfunc::getRunPath() + "/" + filename + ".json");
    //         qWarning(QString(myfunc::getRunPath() + "/" + file + ".json").toStdString().c_str());
    if (!loadFile.exists())
    {
        loadFile.open(QIODevice::NewOnly);
    }
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning(QString("Couldn't open save file: " + filename).toStdString().c_str());
        return false;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    read(loadDoc.object());
    QTextStream(stdout) << "Loaded file: " << filename << " successfully."
                        << " using "
                        << "JSON...\n";
    return true;
}

bool BaseJSONHelper::save()
{
    QFile saveFile(myfunc::getRunPath() + "/" + filename + ".json");
    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning(QString("Couldn't open save file: " + filename).toStdString().c_str());
        return false;
    }

    QJsonObject gameObject;
    write(gameObject);
    QJsonDocument saveDoc(gameObject);
    saveFile.write(saveDoc.toJson());
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------

SingleMaze::SingleMaze()
    :sp(nullptr)
{

}

SingleMaze::SingleMaze(const MazeData & data)
    :sp(new MazeData(data))
{
}

bool SingleMaze::reset(MazeData *p)
{
    if(p ==nullptr){
        sp.reset();
    }else{
        sp.reset(p);
    }
    return true;
}

bool SingleMaze::setId(int _id)
{
    bool ret = false;
    if(sp){
        sp->id = _id;
        ret = true;
    }
    return ret;
}

bool SingleMaze::setName(const QString &_name)
{
    bool ret = false;
    if(sp){
        sp->name = _name;
        ret = true;
    }
    return ret;
}

bool SingleMaze::setBeginStageId(int bid)
{
    bool ret = false;
    if(sp){
        sp->beginStageId = bid;
        ret = true;
    }
    return ret;
}
bool SingleMaze::setTickets(const QMap<int,int>& _tickets)
{
    bool ret = false;
    if(sp){
        sp->tickets = _tickets;
        ret = true;
    }
    return ret;
}

bool SingleMaze::genStage(int stageId,const  MazeStagePos &pos)
{
    if(sp){
        auto it = sp->stages.find(stageId);
        if(it != sp->stages.end()){
            qWarning("已经存在该stageId: %d", stageId);
            return false;
        }
        sp->stages.insert(stageId, pos );
        return true;
    }
    return false;
}

bool SingleMaze::delStage(int stageId)
{
    if(sp){
        auto it = sp->stages.find(stageId);
        if(it != sp->stages.end()){
            sp->stages.erase(it);
        }
        return true;
    }
    return false;
}

bool SingleMaze::setStage(int stageId, const MazeStagePos &pos)
{
    if(sp){
        auto it = sp->stages.find(stageId);
        if(it != sp->stages.end()){
            sp->stages[stageId] = pos;
        }else{
            sp->stages.insert(stageId, pos );
        }
        return true;
    }
    return false;
}

int SingleMaze::genStageId() const
{
    int ret = -1;
    if(!sp){
        qWarning("没有加载数据，无法生成");
        return ret;
    }
    if(sp->stages.size() == 0){
        ret = sp->id + 1;
    }else{
        int maxId = 0;
        auto keys = sp->stages.keys();
        for(auto id : keys){
            if(maxId < id){
                maxId = id;
            }
        }
        ret = ++maxId;
    }
    return ret;
}

///---------------------------------------------------------------------------------------------------------------------------

MazeHelper::MazeHelper()
    :BaseJSONHelper ("maze_data"),currId(0)
{

}

MazeHelper::~MazeHelper()
{

}

MazeHelper *MazeHelper::getInstance()
{
    static MazeHelper *p = new MazeHelper();
    return p;
}

void MazeHelper::read(const QJsonObject &json)
{
    if (json.isEmpty())
    {
        return;
    }
    decltype (datamap) copy;
    auto allMazeIds = json.keys();
    for(auto mazeId : allMazeIds){
        auto mazeJson =  json[mazeId].toObject();
        if(!mazeJson.isEmpty()){
            MazeData data;
            data.potId = mazeJson["potId"].toInt();
            data.name =  mazeJson["name"].toString();
            data.id = mazeJson["id"].toInt();
            data.beginStageId = mazeJson["beginStageId"].toInt();
            auto ticketsJson = mazeJson["tickets"].toObject();
            if(!ticketsJson.isEmpty()){
                auto itemIds = ticketsJson.keys();
                for(auto it = itemIds.cbegin();it != itemIds.cend();++it){
                    auto num = ticketsJson[*it].toInt();
                    if(num > 0){
                        data.tickets.insert(it->toInt(),num);
                    }
                }
            }
            auto stagesJson = mazeJson["stages"].toObject();
            if(!stagesJson.isEmpty()){
                auto stageIds = stagesJson.keys();
                for(auto it = stageIds.cbegin();it != stageIds.cend();++it){
                    auto posJson = stagesJson[*it].toObject();
                    if(!posJson.isEmpty()){
                        auto pos = MazeStagePos(posJson["x"].toInt(), posJson["y"].toInt());
                        data.stages.insert(it->toInt(),pos);
                    }
                }
            }
            copy.insert(data.id, SingleMaze(data));
        }
    }
    datamap = copy;
}

void MazeHelper::write(QJsonObject &json)
{
    for(auto itf = datamap.cbegin();itf != datamap.cend();++itf){
        QJsonObject mazeJson ;
        const auto &maze = itf.value().get();
        mazeJson["id"] = maze.id;
        mazeJson["name"] = maze.name;
        mazeJson["potId"] = maze.potId;
        mazeJson["beginStageId"] =maze.beginStageId;
        QJsonObject tickets;
        for(auto it =maze.tickets.cbegin();it != maze.tickets.cend();++it){
            tickets[QString::number(it.key())] = it.value();
        }
        mazeJson["tickets"] = tickets;

        QJsonObject stages;
        for(auto it =maze.stages.cbegin();it != maze.stages.cend();++it){
            QJsonObject pos;
            pos["x"] = it.value().x;
            pos["y"] = it.value().y;
            stages[QString::number(it.key())] = pos;
        }
        mazeJson["stages"] = stages;

        json[QString::number(maze.id)] = mazeJson;
    }
}

void MazeHelper::create(const MazeData&data)
{
    datamap.insert(data.id,SingleMaze(data));
}

SingleMaze &MazeHelper::getCurrMaze()
{
    if(currId == 0){
        qFatal("当前没有打开的迷宫");
    }
    return datamap[currId];
}

bool MazeHelper::setCurrMaze(int id)
{
    if(id !=0){
        if(datamap.find(id) == datamap.end()){
            qWarning("迷宫数据中不存在id: %d 的迷宫！！！", id);
            return false;
        }
    }
    currId = id;
    return true;
}

bool MazeHelper::isAlreadyExist()
{
    return currId != 0;
}

QMap<int,QString> MazeHelper::getBriefInfo() const{
    QMap<int,QString> info;
    for(auto it = datamap.cbegin();it != datamap.cend();++it){
        info.insert(it.key(),it.value().get().name);
    }
    return info;
}

///@brief 检测mazeid,potid,name,是否有效
std::tuple<bool,QString> MazeHelper::checkIsValid(const MazeData &data)
{
    bool result = true;
    QString errmsg;
    for(auto it = datamap.cbegin();it != datamap.cend();++it){
        auto &maze = it.value().get();
        if(maze.id == data.id){
            result = false;
            errmsg.append("该迷宫 id 已经存在了，请重新输入");
            break;
        }
        if(maze.name == data.name){
            result = false;
            errmsg.append("该迷宫 name 已经存在了，请重新输入");
            break;
        }
        if(maze.potId == data.potId){
            result = false;
            errmsg.append("该 potId 已经被其他的迷宫占用，请重新输入");
            break;
        }
    }
    return std::make_tuple(result,errmsg);
}

std::tuple<bool,QString> MazeHelper::modifyIsValid(int originalId, const MazeData &data){
    bool result = true;
    QString errmsg;
    for(auto it = datamap.cbegin();it != datamap.cend();++it){
        if(it.key() == originalId){
            continue;
        }
        auto &maze = it.value().get();
        if(maze.id == data.id){
            result = false;
            errmsg.append("该迷宫 id 已经存在了，请重新输入");
            break;
        }
        if(maze.name == data.name){
            result = false;
            errmsg.append("该迷宫 name 已经存在了，请重新输入");
            break;
        }
        if(maze.potId == data.potId){
            result = false;
            errmsg.append("该 potId 已经被其他的迷宫占用，请重新输入");
            break;
        }
    }
    return std::make_tuple(result,errmsg);
}
