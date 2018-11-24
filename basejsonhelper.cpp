#include "basejsonhelper.h"
#include "globalfunc.h"
#include <QStringLiteral>
#include <stdexcept>
#include <QJsonDocument>
#include <QtDebug>
#include <QByteArray>
#include <QTextStream>

MazePotData::MazePotData(int _id, const QString &_name, int _potId, const std::map<int, int> &_tickets)
    : id(_id), name(_name), potId(_potId), tickets(_tickets)
{
}

//#pragma region BaseJSONHelper

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

//#pragma endregion

//#pragma region MazePotHelper

MazePotHelper::MazePotHelper()
    : BaseJSONHelper("maze_pot")
{
}

void MazePotHelper::read(const QJsonObject &json)
{
    if (json.isEmpty())
    {
        return;
    }
    decltype(data) copy;
    auto slist = json.keys();
    for (auto it = slist.begin(); it != slist.end(); ++it)
    {
        if (json[*it].isObject())
        {
            std::shared_ptr<MazePotData> sp(new MazePotData());
            auto subJson = json[*it].toObject();
            if (subJson.isEmpty())
            {
                continue;
            }
            if (subJson.contains("id"))
            {
                sp->id = subJson["id"].toInt();
            }
            if (subJson.contains("name") && subJson["name"].isString())
            {
                sp->name = subJson["name"].toString();
            }
            if (subJson.contains("potId"))
            {
                sp->potId = subJson["potId"].toInt();
            }
            if (subJson.contains("tickets") && subJson["tickets"].isObject())
            {
                auto ticketsObj = subJson["tickets"].toObject();
                if (!ticketsObj.isEmpty())
                {
                    std::map<int, int> container;
                    auto itemIds = ticketsObj.keys();
                    for (auto it2 = itemIds.begin(); it2 != itemIds.end(); ++it2)
                    {
                        auto num = ticketsObj[*it2].toInt();
                        if (num > 0)
                        {
                            container.insert({(*it2).toInt(), num});
                        }
                    }
                    sp->tickets = container;
                }
            }
            copy.insert({sp->id, sp});
        }
    }
    data = copy;
}

void MazePotHelper::write(QJsonObject &json)
{
    if (data.size() == 0)
    {
        return;
    }
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        QJsonObject stage;
        stage["id"] = it->second->id;
        stage["name"] = it->second->name;
        stage["potId"] = it->second->potId;

        if (it->second->tickets.size() > 0)
        {
            QJsonObject tickets;
            for (auto it2 = it->second->tickets.begin(); it2 != it->second->tickets.end(); ++it2)
            {
                if (it2->first != 0 && it2->second > 0)
                {
                    tickets[QString::number(it2->first, 10)] = it2->second;
                }
            }
            stage["tickets"] = tickets;
        }
        json[QString::number(it->first, 10)] = stage;
    }
}

std::tuple<bool, QString> MazePotHelper::setMazePot(const MazePotData &mazepot)
{
    auto result = checkIsValid(mazepot);
    if (!std::get<0>(result))
    {
        return result;
    }
    auto it = data.find(mazepot.id);
    if (it != data.end())
    {
        auto &val = data.at(mazepot.id);
        val.reset(new MazePotData(mazepot));
    }
    else
    {
        data.insert({mazepot.id, std::shared_ptr<MazePotData>(new MazePotData(mazepot))});
    }
    return result;
}

std::shared_ptr<MazePotData> MazePotHelper::getMazePot(int id)
{
    auto it = data.find(id);
    if (it != data.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

std::tuple<bool, QString> MazePotHelper::checkIsValid(const MazePotData &maze)
{
    bool result(true);
    QString errormsg;
    if (data.size() == 0)
    {
        if (data.find(maze.id) != data.end())
        {
            errormsg.append("迷宫id已经被使用：" + QString::number(maze.id) + "\n");
            result = false;
        }

        for(auto it = data.begin(); it != data.end();++it){
            if (it->second->name == maze.name)
            {
                errormsg.append("迷宫名称已经被使用：" + maze.name + "\n");
                result = false;
                break;
            }
        }

        for(auto it = data.begin(); it != data.end();++it){
            if (it->second->potId == maze.potId)
            {
                errormsg.append("该potId已经存在迷宫: " + QString::number( maze.potId));
                result = false;
                break;
            }
        }
    }
    return std::make_tuple(result, errormsg);
}

//#pragma endregion
