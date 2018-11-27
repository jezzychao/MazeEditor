#include <QApplication>
#include "formmgr.h"

#include "excelres.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FormMgr::getInstance()->open("mazeeditor");

//    ExMazeOption ex;
//    ex.openExcel(EXCEL_FILES::mazeOption);
//    ex.loadExcel();

//    MazeOption data;
//    data.id = 1001;
//    data.text = "向前走";
//    data.linkStageId = 1001;
//    data.events = QStringList({"123","456"});
//    data.activecond = "1&&2";
//    data.isonlyonce = false;
//    ex.set(1002,new MazeOption(data));

//    ex.saveExcel();
//    ex.closeExcel();
    return a.exec();
}
