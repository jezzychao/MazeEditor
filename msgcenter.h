#ifndef MSGCENTER_H
#define MSGCENTER_H

#include <functional>
#include <memory>
#include <unordered_map>
#include <string>
#include <QString>
#include <QMap>

struct BaseMsg;

class MsgCenter
{
    typedef std::function<void(const std::string &,const BaseMsg &)> Func;
    typedef std::unordered_map<std::string, Func> Container;

  public:
    static MsgCenter *getInstance();
    void attach(const std::string &, Func);
    void detach(const std::string &);
    void notify(const std::string &, const BaseMsg &);

  private:
    MsgCenter();
    std::shared_ptr<Container> sp;
};

//#pragma region Keys

enum class MsgKeys
{
    Non,
    CloseTipsDlg,
    OpenTipsDlg,
    OpenSetBasicInfo,
    ConfirmModifyBasicInfo
};

std::string key2str(MsgKeys k);

//#pragma endregion

//#pragma region Msg Struct

struct BaseMsg
{
};

struct MsgNull : public BaseMsg
{
};

struct MsgOpenTipsDlg : public BaseMsg
{
    std::string text;
};

struct MsgCloseTipsDlg : public BaseMsg
{
    bool status;
};

struct MsgSetBasicInfo : public BaseMsg
{
    bool isEmpty;
    int id;
    QString name;               //迷宫的名称，用于区分不同的迷宫
    int potId;                  //迷宫所在的pot点id
    QMap<int, int> tickets; //进入迷宫所需的门票，没有就是空容器
};

template <typename T>
const T &transfer(const BaseMsg &cm);

template <typename T>
T &transfer(BaseMsg &cm);

//#pragma endregion

#endif // MSGCENTER_H
