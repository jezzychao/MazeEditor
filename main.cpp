#include <QApplication>
#include "formmgr.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormMgr::getInstance()->open("mazeeditor");
    return a.exec();
}
