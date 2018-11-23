#ifndef MSGCENTER_H
#define MSGCENTER_H

#include <functional>
#include <memory>
#include <unordered_map>
struct BaseMsg;

class MsgCenter
{
    typedef std::function<void(const BaseMsg&)> Func;
    typedef std::unordered_map<std::string, Func> Container;
public:
    static  MsgCenter *getInstance();
    void attach(const std::string &, Func );
    void detach(const std::string &);
    void notify(const std::string &, const BaseMsg&);
private:
    MsgCenter();
    std::shared_ptr<Container> sp;
};

#endif // MSGCENTER_H
