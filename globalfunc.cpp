#include "globalfunc.h"
#include <QApplication>
using namespace myfunc;

const QString &myfunc::getRunPath(){
    static  QString cs = qApp->applicationDirPath();
    return cs;
}

const QString &myfunc::getExcelFilesPath()
{
    static  QString s;
    if(s.isEmpty()){
        s = qApp->applicationDirPath();
        //E:\Workspace\HY2\荒野求生2\配置表\excel
        s.replace("maze_editor","excel");
//                s.replace("debug","excel");
    }
    return s;
}

const QString &myfunc::getPublishPath()
{
    static  QString s;
    if(s.isEmpty()){
        s = qApp->applicationDirPath();
        s.replace("maze_editor","result");
//                 s.replace("debug","result");
    }
    return s;
}
