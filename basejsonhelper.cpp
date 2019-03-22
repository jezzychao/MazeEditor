#include "basejsonhelper.h"
#include "globalfunc.h"
#include <QStringLiteral>
#include <stdexcept>
#include <QJsonDocument>
#include <QtDebug>
#include <QByteArray>
#include <QTextStream>
#include <QFile>
#include <QJsonArray>

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
    qWarning("save json successfully!!!");
    return true;
}

//---------------------------------------------------------------------------------------------------------------------------

MazeHelper::MazeHelper()
    :BaseJSONHelper ("maze_data_editor"),currId(0)
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
    QMap<int ,std::shared_ptr<MazeData>> temp;
    auto allMazeIds = json.keys();
    for(auto mazeId : allMazeIds){
        auto mazeJson =  json[mazeId].toObject();
        if(!mazeJson.isEmpty()){
            MazeData data;
            data.potId = mazeJson["potId"].toInt();
            data.name =  mazeJson["name"].toString();
            data.bgi = mazeJson["bgi"].toString();
             data.bgm = mazeJson["bgm"].toString();
            data.id = mazeJson["id"].toInt();
            data.beginStageId = mazeJson["beginStageId"].toInt();
            data.endStageId = mazeJson["endStageId"].toInt();
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
                for(auto v: stageIds){
                    MazeStage stage;
                    QJsonObject stageJson = stagesJson[v].toObject();
                    if(!stageJson.isEmpty()){
                        readStage(stage, stageJson);
                        data.stages.insert(v.toInt(), stage);
                    }
                }
            }
            temp.insert(data.id, std::make_shared<MazeData>(std::move(data)));
        }
    }

    auto oldMaze = m_maze;
    m_maze = temp;
    oldMaze.clear();
}

void MazeHelper::write(QJsonObject &json)
{
    for(auto itf = m_maze.cbegin();itf != m_maze.cend();++itf){
        QJsonObject mazeJson ;
        const auto &maze = itf.value().get();
        mazeJson["id"] = maze->id;
        mazeJson["name"] = maze->name;
        mazeJson["potId"] = maze->potId;
        mazeJson["bgi"] = maze->bgi;
         mazeJson["bgm"] = maze->bgm;
        mazeJson["beginStageId"] =maze->beginStageId;
        mazeJson["endStageId"] = maze->endStageId;
        QJsonObject tickets;
        for(auto it =maze->tickets.cbegin();it != maze->tickets.cend();++it){
            tickets[QString::number(it.key())] = it.value();
        }
        mazeJson["tickets"] = tickets;

        QJsonObject stages;
        for(auto it =maze->stages.cbegin();it != maze->stages.cend();++it){
            QJsonObject stageJson;
            writeStage(*it, stageJson);
            if(!stageJson.isEmpty()){
                stages[QString::number(it->id)] = stageJson;
            }
        }
        mazeJson["stages"] = stages;

        json[QString::number(maze->id)] = mazeJson;
    }
}

void MazeHelper::readStage(MazeStage &stage,const QJsonObject &json) const
{
    if(json.isEmpty()){
        return;
    }
    stage.id = json["id"].toInt();
    stage.type = json["type"].toInt();
    stage.mappingId = json["mappingId"].toInt();
    stage.name = json["name"].toString();
    stage.sprite = json["sprite"].toString();
    stage.desc = json["desc"].toString();
    stage.entryStoryId = json["entryStoryId"].toInt();
    stage.remark = json["remark"].toString();

    QMap<int,MazeOption> temp;
    auto allOpJson = json["options"].toObject();
    if(!allOpJson.isEmpty()){
        auto allOpIds = allOpJson.keys();
        for(auto val : allOpIds){
            QJsonObject subJson = allOpJson[val].toObject();
            if(!subJson.isEmpty()){
                MazeOption option;
                readOption(option, subJson);
                temp.insert(option.id,option);
            }
        }
    }
    stage.options = temp;

    QPoint point( json["x"].toInt(),  json["y"].toInt());
    stage.pos = point;

    decltype (stage.frontStageIds) tempfronts;
    auto fronts = json["frontStageIds"].toArray();
    for(auto v : fronts)
    {
        tempfronts.push_back(v.toInt());
    }
    stage.frontStageIds = tempfronts;

    decltype (stage.nextStageIds) tempnexts;
    auto nexts = json["nextStageIds"].toArray();
    for(auto v : nexts)
    {
        tempnexts.push_back(v.toInt());
    }
    stage.nextStageIds = tempnexts;
}

void MazeHelper::writeStage(const MazeStage &stage,QJsonObject &json)
{
    json["id"] = stage.id;
    json["type"] = stage.type;
    json["mappingId"] = stage.mappingId;
    json["name"] = stage.name;
    json["sprite"] = stage.sprite;
    json["desc"] = stage.desc;
    json["entryStoryId"] = stage.entryStoryId;
    json["remark"] = stage.remark;

    QJsonObject subJson;
    for(const auto &v: stage.options){
        QJsonObject optionJson;
        writeOption(v, optionJson);
        if(!optionJson.isEmpty()){
            subJson[QString::number(v.id)] = optionJson;
        }
    }
    json["options"] = subJson;

    json["x"] = stage.pos.x();
    json["y"] = stage.pos.y();

    QJsonArray arr;
    for(auto v: stage.frontStageIds){
        arr.append(v);
    }
    json["frontStageIds"]  = arr;

    QJsonArray arr2;
    for(auto v: stage.nextStageIds){
        arr2.append(v);
    }
    json["nextStageIds"]  = arr2;
}

void MazeHelper::readOption(MazeOption &option,const QJsonObject &json)const
{
    if(json.isEmpty()){
        return;
    }
    option.id = json["id"].toInt();
    option.text = json["text"].toString();
    option.activecond = json["activecond"].toString();
    option.disabledTips = json["disabledTips"].toString();
    option.linkStageId = json["linkStageId"].toInt();
    option.isonlyonce = json["isonlyonce"].toInt();
    option.remark = json["remark"].toString();

    decltype (option.attrsChanged) tempAttr;
    auto attrArray = json["attrsChanged"].toArray();
    for(auto v :attrArray){
        tempAttr.push_back(v.toString());
    }
    option.attrsChanged = tempAttr;

    decltype (option.events) temp;
    auto jsarr = json["events"].toArray();
    for(auto v :jsarr){
        temp.push_back(v.toInt());
    }
    option.events = temp;
}

void MazeHelper::writeOption(const MazeOption &option,QJsonObject &json)
{
    json["id"] = option.id;
    json["text"] = option.text;
    json["activecond"] = option.activecond;
    json["disabledTips"] = option.disabledTips;
    json["linkStageId"] = option.linkStageId;
    json["isonlyonce"] = option.isonlyonce;
    json["remark"] = option.remark;

    QJsonArray attrArr;
    for(auto v :option.attrsChanged){
        attrArr.push_back(v);
    }
    json["attrsChanged"] = attrArr;

    QJsonArray arr;
    for(auto v :option.events){
        arr.push_back(v);
    }
    json["events"] = arr;
}

void MazeHelper::create(const MazeData&data)
{
    m_maze.insert(data.id,std::make_shared<MazeData>(data));
}

MazeData MazeHelper::copyCurrMaze()const
{
    return getCurrMaze();
}

const MazeData &MazeHelper::getCurrMaze() const
{
    if(currId == 0){
        qFatal("当前没有打开的迷宫");
    }
    return *(m_maze[currId]);
}

bool MazeHelper::setCurrMaze(const MazeData &maze)
{
    if(m_maze.find(maze.id) == m_maze.end()){
        m_maze.insert(maze.id, std::make_shared<MazeData>(maze));
    }else{
        m_maze[maze.id].reset(new MazeData(maze));
    }
    return true;
}

bool MazeHelper::setCurrMaze(int id)
{
    if(id !=0){
        if(m_maze.find(id) == m_maze.end()){
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
    for(auto it = m_maze.cbegin();it != m_maze.cend();++it){
        info.insert(it.key(),it.value()->name);
    }
    return info;
}

///@brief 检测mazeid,potid,name,是否有效
std::tuple<bool,QString> MazeHelper::checkIsValid(const MazeData &data)
{
    bool result = true;
    QString errmsg;
    for(auto it = m_maze.cbegin();it != m_maze.cend();++it){
        const auto &maze = *(it.value());
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
    for(auto it = m_maze.cbegin();it != m_maze.cend();++it){
        if(it.key() == originalId){
            continue;
        }
        const auto &maze = *(it.value());
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

int MazeHelper::genNewStageId() const
{
    int mazeId = currId;
    auto funcGenId = [mazeId](int maxId = 0){
        return  maxId? maxId + 1  : mazeId * 10 + 1;
    };
    if(m_maze.size() == 0 || m_maze.find(mazeId) == m_maze.end()){
        return funcGenId();
    }else{
        auto &stages = m_maze[mazeId]->stages;
        int maxId = 0;
        auto keys = stages.keys();
        for(auto id : keys){
            if(maxId < id){
                maxId = id;
            }
        }
        return funcGenId(maxId);
    }
}

void MazeHelper::setStage(MazeStage &stage)
{
    //1.先断开原先的stage关联后置id，2.重新连接新的后置id，3.对原先的stage其他字段赋值
    //同时对该stage的nextStageIds和后置stage的frontStageIds进行了操作
    auto &curMaze = m_maze[currId];
    bool isExisted = curMaze->stages.find(stage.id) != curMaze->stages.end();

    //step.1
    if(isExisted){
        auto &originStage = curMaze->stages[stage.id];
        for(auto nextId: originStage.nextStageIds){
            auto &frontIds = curMaze->stages[nextId].frontStageIds;
            for(auto frontId: frontIds){
                if(frontId == originStage.id){
                    frontIds.removeOne(frontId);
                    break;
                }
            }
        }
    }

    //step.2
    for(auto nextId: stage.nextStageIds){
        curMaze->stages[nextId].frontStageIds.push_back(stage.id);
    }

    //step.3
    if(isExisted){
        curMaze->stages[stage.id] = stage;
    }else{
        curMaze->stages.insert(stage.id, stage);
    }
}

MazeStage MazeHelper::getStage(int id)
{
    const auto &maze = getCurrMaze();
    auto it = maze.stages.find(id);
    if(it == maze.stages.end()){
        qFatal("not exist stage id: %d",id);
    }
    return maze.stages[id];
}

QMap<int,QString> MazeHelper::getStageInfos(std::initializer_list<int> excludeIds)const
{
    QMap<int,QString> ret;
    auto funcIsOk = [&excludeIds](int stageId)->bool{
        for(auto it = excludeIds.begin();it != excludeIds.end();++it){
            if(stageId == *it){
                return false;
            }
        }
        return true;
    };
    const auto &maze = getCurrMaze();
    for(const auto &stage: maze.stages){
        if(funcIsOk(stage.id)){
            ret.insert(stage.id,stage.remark);
        }
    }
    return ret;
}

QVector<int> MazeHelper::findFrontStageIds(int )
{
    return {};
}

QVector<int> MazeHelper::findNextStageIds(int )
{
    return {};
}

int MazeHelper::genNewOptionId(const MazeStage &stage)const
{
    auto funcGenId = [&stage](int maxId = 0){
        return  maxId? maxId + 1  : stage.id * 10 + 1;
    };
    int maxId = 0;
    auto keys = stage.options.keys();
    for(auto id : keys){
        if(maxId < id){
            maxId = id;
        }
    }
    return funcGenId(maxId);
}

void MazeHelper::setOption(MazeOption &option)
{
    if(currId == 0){
        qFatal("Have not opened maze");
    }
    auto &currMaze = m_maze[currId];
    for(auto it =currMaze->stages.cbegin();it != currMaze->stages.cend();++it){
        const auto &ops = it.value().options;
        for(auto it_op = ops.begin();it_op != ops.end();++it_op){
            if(option.id == it_op->id){
                currMaze->stages[it.key()].options[it_op.key()] = option;
                return ;
            }
        }
    }
    qFatal("Do not exist optionId: %d in maze: %d",option.id,currMaze->id);
}

MazeOption MazeHelper::getOption(int optionId)
{
    if(currId == 0){
        qFatal("Have not opened maze");
    }
    auto &currMaze = m_maze[currId];

    for(auto it =currMaze->stages.cbegin();it != currMaze->stages.cend();++it){
        const auto &ops = it.value().options;
        for(auto it_op = ops.cbegin();it_op != ops.cend();++it_op){
            if(optionId == it_op->id){
                return it_op.value();
            }
        }
    }
    qFatal("Do not exist optionId: %d in maze: %d",optionId,currMaze->id);
}

void MazeHelper::deleteMaze(int mazeId)
{
    if(currId == 0){
        qFatal("Have not opened maze");
    }
    auto it = m_maze.find(mazeId);
    if(it != m_maze.end()){
        m_maze.erase(it);
    }
}

std::tuple<bool,QString> MazeHelper::checkStageIsValid(const MazeStage &stage)
{
    bool status = false;
    QString errmsg;
    if(stage.type == 1|| stage.type == 6 || stage.type == 7)
    {
        if(stage.name.isEmpty()){
            errmsg += ("没有配置名称\n");
        }
        if(stage.sprite.isEmpty()){
            errmsg += ("没有配置图片\n");
        }
        if(stage.desc.isEmpty()){
            errmsg += ("没有配置描述\n ");
        }
    }
    else if(stage.type == 2 || stage.type == 3 || stage.type == 4){
        if(stage.mappingId == 0){
             errmsg += ("该 stage 类型没有关联 stage.xlsx 中的 id\n");
        }
    }
    else if(stage.type == 5){
        if(stage.mappingId == 0){
             errmsg += ("该 stage 类型没有关联 minigames.xlsx 中的 id\n");
        }
    }
    else
    {
        errmsg = "当前 Stage 的类型配置错误: type: " + QString::number(stage.type) + "\n";
    }

    if(stage.options.size() == 0){
        errmsg += ("没有配置任何选项信");
    }else{
        for(auto it = stage.options.cbegin();it != stage.options.cend();++it){
            auto result = checkOptionIsValid(stage.type,it.value());
            if(!std::get<1>(result).isEmpty()){
                errmsg.append("\n").append(std::get<1>(result));
            }
        }
    }
    status = errmsg.isEmpty()? true :false;

    return std::make_tuple(status, errmsg);
}

std::tuple<bool,QString> MazeHelper::checkOptionIsValid(int stageType ,const MazeOption &option)
{
    bool status = false;
    QString errmsg;
    if(option.linkStageId == -1){
        errmsg +=("optionId: " +QString::number(option.id) + ", 没有后置的stageId\n");
    }
    if(stageType == 1){
        if(option.text.isEmpty()){
            errmsg +=("没有对应的选项文本\n");
        }
    }
     status = errmsg.isEmpty()? true :false;

    return std::make_tuple(status, errmsg);
}
