#include <QApplication>
#include "formmgr.h"
#include "excelres.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormMgr::getInstance()->open("mazeeditor");
    {
    ExcelMazeOption excel("text.xlsx");
    if(excel.exists()){
         excel.loadExcel();
         MazeOption data;
         data.id = 1001;
         data.text =  "往前走";
         data.disabledTips = "激活提示";
         data.activecond = "激活条件";
         data.linkStageId = 60101;
         data.events = QStringList({"8888","9999"});
         data.isonlyonce = true;
         excel.set(1001, new MazeOption(data));
         excel.saveExcel();
    }else{
        excel.createExcel({
                              std::make_tuple("id",FIELD_TYPE::INT),
                              std::make_tuple("text",FIELD_TYPE::STR),
                              std::make_tuple("activecond",FIELD_TYPE::STR),
                              std::make_tuple("disabledTips",FIELD_TYPE::STR),
                              std::make_tuple("linkStageId",FIELD_TYPE::INT),
                              std::make_tuple("events",FIELD_TYPE::LIST),
                              std::make_tuple("isonlyonce",FIELD_TYPE::INT)
        });
        MazeOption data;
        data.id = 1002;
        data.text =  "往前走";
        data.disabledTips = "激活提示";
        data.activecond = "激活条件";
        data.linkStageId = 60101;
        data.events = QStringList({"8888","9999"});
        data.isonlyonce = true;
        excel.set(1002, new MazeOption(data));
        excel.saveExcel();
    }}

    return a.exec();
}
