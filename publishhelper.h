#ifndef PUBLISHHELPER_H
#define PUBLISHHELPER_H

#include <QJsonObject>
#include <QStringList>

class Exlocalization;

class PublishHelper
{
public:
    PublishHelper();
    ~PublishHelper();
    bool exe();
private:
    bool load(QJsonObject &json);
    bool save(const QJsonObject &json);
    bool handle(QJsonObject &json);
    bool handlerOption(QJsonObject &json);
    bool handlerStage(QJsonObject &json);
    bool handleMaze(QJsonObject &json);
    QString backupPath;
    QString publishPath;
    QString filePath;
    Exlocalization *excel;
    ///@brief 已经被使用的多语言ID
    QStringList *usedIds;
};

#endif // PUBLISHHELPER_H
