#include "msgcenter.h"
#include <stdexcept>
#include <QDebug>
#include <QMap>

namespace  {
QMap<MsgKeys, QString> keystr{
    {   MsgKeys::Non, "Non" },
    {   MsgKeys::CloseTipsDlg, "CloseTipsDlg" },
    {   MsgKeys::OpenTipsDlg, "OpenTipsDlg" },
    {   MsgKeys::OpenSetBasicInfoForSet, "OpenSetBasicInfoForSet" },
    {   MsgKeys::OpenSetBasicInfoForNew, "OpenSetBasicInfoForNew" },
    {   MsgKeys::OpenNewMazeView, "OpenNewMazeView" },
    {   MsgKeys::OpenDlgSetOption, "OpenDlgSetOption" },
    {   MsgKeys::OpenDlgSetStage, "OpenDlgSetStage" },
    {   MsgKeys::DeleteRect, "DeleteRect" },
    {   MsgKeys::ConfirmModifyBasicInfo, "ConfirmModifyBasicInfo" },
    {   MsgKeys::ConfirmOpenMaze, "ConfirmOpenMaze" }
};
}

std::string key2str(MsgKeys k)
{
    return keystr[k].toStdString();
}

template <typename T>
const T &transfer(const BaseMsg &cm)
{
    auto &val = static_cast<const T &>(cm);
    return val;
}

template <typename T>
T &transfer(BaseMsg &cm)
{
    auto &val = static_cast<T &>(cm);
    return val;
}

MsgCenter::MsgCenter()
    : sp(new Container)
{
}

MsgCenter *MsgCenter::getInstance()
{
    static MsgCenter *p = new MsgCenter;
    return p;
}

void MsgCenter::attach(const std::string &k, Func f)
{
    if (sp->find(k) == sp->end())
    {
        sp->insert({k, f});
    }
    else
    {
        qFatal("Already exist key: %s",k.c_str());
    }
}

void MsgCenter::detach(const std::string &k)
{
    auto it = sp->find(k);
    if (it != sp->end())
    {
        sp->erase(it);
    }
}

void MsgCenter::notify(const std::string &k, const BaseMsg &msg)
{
    auto it = sp->find(k);
    if (it != sp->end())
    {
        it->second(k,msg);
    }
}
