#include "msgcenter.h"
#include <stdexcept>

MsgCenter::MsgCenter()
    :sp(new Container)
{
}

MsgCenter *MsgCenter::getInstance()
{
    static MsgCenter *p = new MsgCenter;
    return p;
}

void MsgCenter:: attach(const std::string &k, Func f )
{
    if(sp->find(k) == sp->end()){
        sp->insert({k,f});
    }else{
     throw new std::runtime_error("Already exist key: " + k);
    }
}

void MsgCenter::detach(const std::string &k)
{
    auto it = sp->find(k);
    if(it != sp->end()){
        sp->erase(it);
    }
}

void MsgCenter::notify(const std::string &k, const BaseMsg &msg)
{
    auto it = sp->find(k);
    if(it != sp->end()){
        it->second(msg);
    }
}
