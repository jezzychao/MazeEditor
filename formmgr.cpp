#include "formmgr.h"
#include "mazeeditor.h"
#include "setbasicinfo.h"
#include <stdexcept>
#include <utility>

FormMgr::FormMgr()
{

}

FormMgr *FormMgr::getInstance()
{
    static FormMgr *p = new FormMgr();
    return p;
}

void FormMgr::open(const std::string &name)
{
    auto it = smap.find(name);
    if(it != smap.end()){
        if(!it->second->isVisible()){
             it->second->show();
        }
    }else{
        if(auto ptr =  createForm(name)){
            ptr->show();
            smap.insert(std::make_pair(name, std::move(ptr)));
        }
    }
}

void FormMgr::close(const std::string &name)
{
    auto it = smap.find(name);
    if(it != smap.end()){
        if(it->second->close()){
             smap.erase(it);
        }else{
            throw std::runtime_error("close form: " + name + " failure.");
        }
    }
}

 std::unique_ptr<QWidget> createForm(const std::string &name)
{
    if(name == "mazeeditor"){
        return std::unique_ptr<QWidget>(new mazeeditor());
    }else if(name == "setbasicinfo"){
         return std::unique_ptr<QWidget>(new SetBasicInfo());
    }
    return nullptr;
}
