#ifndef GLOBALFUNC_H
#define GLOBALFUNC_H
#include <QString>

//只初始化一次
namespace  myfunc  {
    const QString &getRunPath();

    const QString &getExcelFilesPath();
}


#endif // GLOBALFUNC_H
