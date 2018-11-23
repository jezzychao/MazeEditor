#ifndef KEYS_H
#define KEYS_H
#include <string>
enum  class MsgKeys
{
    non
};

std::string key2str(MsgKeys k)
{
    switch (k) {
    case MsgKeys::non: return "non";
    }
    return "error";
}

#endif // KEYS_H
