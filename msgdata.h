#ifndef MSGDATA_H
#define MSGDATA_H
#include <string>

struct BaseMsg{
};

struct MsgTest: public BaseMsg
{
    std::string s1;
};

template<typename T>
T  transfer(const BaseMsg &cm){
    auto val = static_cast<const  T &>(cm);
    return val;
}

#endif // MSGDATA_H
