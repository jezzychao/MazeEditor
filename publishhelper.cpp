#include "publishhelper.h"
#include <QApplication>
#include "globalfunc.h"
#include <QFile>
#include <QJsonDocument>
#include "excelres.h"
#include "dlgconfirm.h"

PublishHelper::PublishHelper()
    :backupPath(myfunc::getRunPath()+ "/maze_data.json"),
      publishPath(myfunc::getPublishPath() + "/maze_data.json"),
      filePath(myfunc::getRunPath()+ "/maze_data_editor.json" ),
      excel(new Exlocalization),
      usedIds(new QStringList)
{
    excel->openExcel(EXCEL_FILES::localization);
    excel->setSheet(3);
    excel->loadExcel();
}

PublishHelper::~PublishHelper()
{
    delete excel;
    delete usedIds;
}

bool   PublishHelper::exe()
{
    QJsonObject json;
    if(load(json) && handle(json) &&save(json)){
        excel->removeUnused(*usedIds);
        excel->saveExcel();
        return  true;
    }
    return false;
}

bool PublishHelper::load(QJsonObject &json)
{
    QFile loadFile(filePath);
    if (!loadFile.exists())
    {
        qWarning(QString("publish failed.\n"+filePath + " do not exists!!!").toStdString().c_str());
        return false;
    }
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning(QString("Couldn't open save file: " + filePath).toStdString().c_str());
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    json = loadDoc.object();
    return true;
}

bool PublishHelper::save(const QJsonObject &json)
{
    QFile saveFile(publishPath);
    if(!saveFile.exists()){
        saveFile.open(QIODevice::NewOnly);
    }else{
        if (!saveFile.open(QIODevice::WriteOnly))
        {
            qWarning(QString("Couldn't open save file: " + publishPath).toStdString().c_str());
            return false;
        }
    }
    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());

    QFile copyFile(backupPath);
    if(copyFile.exists()){
        copyFile.remove();
    }
    saveFile.copy(backupPath);
    qDebug("save json successfully!!!");
    return true;
}

bool  PublishHelper::handlerOption(QJsonObject &json)
{
    if(!json.isEmpty()){
        json.remove("remark");
        auto tips = json["disabledTips"].toString();
        if(!tips.isEmpty()){
            auto localId = excel->getId(tips);
            json["disabledTips"] = localId;
            usedIds->push_back(localId);
        }
        auto text = json["text"].toString();
        if(!text.isEmpty()){
            auto localId = excel->getId(text);
            json["text"] = localId;
            usedIds->push_back(localId);
        }
    }
    return true;
}

bool PublishHelper::handlerStage(QJsonObject &json)
{
    if(!json.isEmpty()){
        json.remove("remark");
        json.remove("x");
        json.remove("y");
        json.remove("frontStageIds");
        json.remove("nextStageIds");

        auto desc = json["desc"].toString();
        if(!desc.isEmpty()){
            auto localId = excel->getId(desc);
            json["desc"] = localId;
            usedIds->push_back(localId);
        }

        auto name = json["name"].toString();
        if(!name.isEmpty()){
            auto localId = excel->getId(name);
            json["name"] = localId;
            usedIds->push_back(localId);
        }

        auto optionsJson = json["options"].toObject();
        if(!optionsJson.isEmpty()){
            auto ids = optionsJson.keys();
            for(auto id: ids){
                auto subJson = optionsJson[id].toObject();
                handlerOption(subJson);
                optionsJson[id] = subJson;
            }
            json["options"] = optionsJson;
        }
    }
    return true;
}

bool PublishHelper::handleMaze(QJsonObject &json)
{
    if(!json.isEmpty()){
        json.remove("endStageId");
        json.remove("name");
        json.remove("potId");
        auto stagesJson = json["stages"].toObject();
        if(!stagesJson.isEmpty()){
            stagesJson.remove("0");
            auto stageIds = stagesJson.keys();
            for(auto stageId: stageIds){
                auto stageJson = stagesJson[stageId].toObject();
                handlerStage(stageJson);
                stagesJson[stageId] = stageJson;
            }
            json["stages"] = stagesJson;
        }
    }
    return true;
}

bool PublishHelper::handle(QJsonObject &json)
{
    if(json.isEmpty()){
        return true;
    }
    auto mazeIds = json.keys();
    for(auto mazeId: mazeIds){
        auto mazeJson = json[mazeId].toObject();
        handleMaze(mazeJson);
        json[mazeId] = mazeJson;
    }
    return true;
}
