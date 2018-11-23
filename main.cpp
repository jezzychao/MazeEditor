#include <QApplication>
#include "formmgr.h"
#include "globalfunc.h"

#include "basejsonhelper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormMgr::getInstance()->open("mazeeditor");
//    MainCtrl::getInstance()->start();
    return a.exec();
}
