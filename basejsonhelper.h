#ifndef BASEJSONHELPER_H
#define BASEJSONHELPER_H

#include <QString>
#include <QJsonObject>
#include <QFile>
#include <map>
#include <unordered_map>
#include <memory>
#include <string>
#include <tuple>

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

struct MazePotData
{
    MazePotData() = default;
    MazePotData(int _id, const QString &_name, int _potId, const std::map<int, int> &_tickets);
    int id;
    QString name;               //迷宫的名称，用于区分不同的迷宫
    int potId;                  //迷宫所在的pot点id
    std::map<int, int> tickets; //进入迷宫所需的门票，没有就是空容器
};

class MazePotHelper : public BaseJSONHelper
{
  public:
    MazePotHelper();
    ~MazePotHelper() override = default;
    std::tuple<bool, QString> setMazePot(const MazePotData &);
    std::shared_ptr<MazePotData> getMazePot(int);

  protected:
    void read(const QJsonObject &) override;
    void write(QJsonObject &) override;

  private:
    std::tuple<bool, QString> checkIsValid(const MazePotData &);
    std::unordered_map<int, std::shared_ptr<MazePotData>> data; //id 2 data
};

#endif // BASEJSONHELPER_H
