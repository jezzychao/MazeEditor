#include "formmgr.h"
#include "mazeeditor.h"
#include "setbasicinfo.h"
#include "tipsdialog.h"
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

std::shared_ptr<QWidget> FormMgr::open(const std::string &name)
{
    auto it = smap.find(name);
    if (it != smap.end())
    {
        if (!it->second->isVisible())
        {
            it->second->show();
        }
        return it->second;
    }
    else
    {
        if (auto ptr = createForm(name))
        {
            ptr->show();
            smap.insert(std::make_pair(name, ptr));
            return ptr;
        }
    }
    return nullptr;
}

void FormMgr::close(const std::string &name)
{
    auto it = smap.find(name);
    if (it != smap.end())
    {
        if (it->second->close())
        {
            smap.erase(it);
        }
        else
        {
            throw std::runtime_error("close form: " + name + " failure.");
        }
    }
//    for(auto it = smap.begin();it != smap.end();++it){
//        QString qs(("form name: " + it->first).c_str());
//        qs.append(", use_count: " + QString::number(it->second.use_count(),10));
//        qWarning(qs.toStdString().c_str());
//    }
}

std::shared_ptr<QWidget> createForm(const std::string &name)
{
    if (name == "mazeeditor")
    {
        return std::shared_ptr<QWidget>(new mazeeditor());
    }
    else if (name == "setbasicinfo")
    {
        return std::shared_ptr<QWidget>(new SetBasicInfo());
    }
    else if (name == "tipsdialog")
    {
        return std::shared_ptr<QWidget>(new TipsDialog());
    }
    return nullptr;
}
