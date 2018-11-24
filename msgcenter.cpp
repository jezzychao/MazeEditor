#include "msgcenter.h"
#include <stdexcept>
#include <QDebug>

std::string key2str(MsgKeys k)
{
    switch (k)
    {
    case MsgKeys::Non:
        return "Non";
    case MsgKeys::CloseTipsDlg:
        return "CloseTipsDlg";
    case MsgKeys::OpenTipsDlg:
        return "OpenTipsDlg";
    }
    return "error";
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
