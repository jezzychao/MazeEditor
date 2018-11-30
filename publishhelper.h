#ifndef PUBLISHHELPER_H
#define PUBLISHHELPER_H

#include <QJsonObject>

class PublishHelper
{
public:
    PublishHelper();
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
};

#endif // PUBLISHHELPER_H
