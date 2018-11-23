#ifndef FORMMGR_H
#define FORMMGR_H
#include <unordered_map>
#include <memory>
#include <QWidget>

class FormMgr
{
public:
    static FormMgr *getInstance();
    void open(const std::string &);
     void close(const std::string &);
private:
    FormMgr();
    std::unordered_map<std::string, std::unique_ptr<QWidget>> smap;
};

 std::unique_ptr<QWidget> createForm(const std::string &);

#endif // FORMMGR_H
