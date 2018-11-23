#ifndef RESEXCEL_H
#define RESEXCEL_H
#include <QAxObject>
#include <memory>
#include <string>

class ResExcel
{
public:
    ResExcel(const std::string &);
    ResExcel(const ResExcel &)=delete;
    ResExcel &operator=(const ResExcel &) = delete;
    ~ResExcel();


private:
    std::shared_ptr<QAxObject> sp;
};

#endif // RESEXCEL_H
