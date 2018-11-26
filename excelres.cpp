#include "excelres.h"
#include "globalfunc.h"
#include <QFile>
#include <QMessageBox>

ExcelRes::ExcelRes()
    :ExcelBase ()
{

}

bool ExcelRes::openExcel(EXCEL_FILES file)
{
    QString name;
    switch(file){
    case EXCEL_FILES::mazeOption:name = "mazeOption"; break;
    case EXCEL_FILES::pot: name = "pot";break;
    case EXCEL_FILES::mazeStage: name = "mazeStage";break;
    }
    QString fullPath = myfunc::getExcelFilesPath() + "/" + name + ".xlsx";
    if(ExcelBase::open(fullPath)){
        switch(file){
        case EXCEL_FILES::mazeOption:
            addFields({
                          std::make_tuple("id",FIELD_TYPE::INT),
                          std::make_tuple("text",FIELD_TYPE::STR),
                          std::make_tuple("activecond",FIELD_TYPE::STR),
                          std::make_tuple("disabledTips",FIELD_TYPE::STR),
                          std::make_tuple("linkStageId",FIELD_TYPE::INT),
                          std::make_tuple("events",FIELD_TYPE::LIST),
                          std::make_tuple("isonlyonce",FIELD_TYPE::INT)
                      });
            break;
        case EXCEL_FILES::pot:
            break;
        case EXCEL_FILES::mazeStage:
            break;
        }
    }
    return true;
}

void ExcelRes::closeExcel(){
    close();
}

bool ExcelRes::addFields(const QVector<std::tuple<QString,FIELD_TYPE>> &fields )
{
    for(auto it = fields.cbegin(); it != fields.cend();++it){
        QString fieldname = std::get<0>(*it);
        auto fieldtype = std::get<1>(*it);
        QString qs;
        switch (fieldtype) {
        case FIELD_TYPE::NON: qs = "";break;
        case FIELD_TYPE::INT:  qs = "INT";break;
        case FIELD_TYPE::STR: qs = "STR";break;
        case FIELD_TYPE::LIST: qs = "LIST";break;
        }
        QVariant qvar(qs);
        auto col = it - fields.cbegin() + 1;
        write(1, col, qvar);
        write(2,col, QVariant("字段描述"));
        write(3, col, QVariant(fieldname));
    }
    return true;
}

ExcelData& ExcelRes::at(int id)
{
    auto it = m_map.find(id);
    if(it == m_map.end()){
        qFatal("excel: %s 中不存在 id: %d 的数据",getFullPath().toStdString().c_str(), id);
    }
    return *(it.value());
}

ExcelData ExcelRes::get(int id)
{
    return at(id);
}

bool ExcelRes::set(int id, ExcelData *pdata)
{
    auto it = m_map.find(id);
    if(it != m_map.end()){
        it.value().reset(pdata);
    }else{
        m_map.insert(id, std::shared_ptr<ExcelData>(pdata));
    }
    return true;
}

template <typename T>
bool ExcelRes::saveExcel()
{
    return false;
}

template <typename T>
void ExcelRes::loadExcel()
{

}

template <>
bool ExcelRes::saveExcel<MazeOption>()
{
    int row = 3;
    for(auto it = m_map.cbegin();it != m_map.cend();++it){
        ++row;
        auto base = it.value();
        auto &data = *base;
        auto &option = static_cast<MazeOption&>(data);
        write(row, 1, QVariant(option.id));
        write(row, 2, QVariant(option.text));
        write(row, 3, QVariant(option.activecond));
        write(row, 4, QVariant(option.disabledTips));
        write(row, 5, QVariant(option.linkStageId));
        write(row, 6, QVariant(option.events));
        write(row, 7, QVariant(option.isonlyonce?1:0));
    }
    save();
    return true;
}

template <>
void ExcelRes::loadExcel<MazeOption>()
{
    m_map.clear();
    QList<QList<QVariant>> cells;
    readAll(cells);
    for(auto it_row = cells.cbegin(); it_row != cells.cend();++it_row){
        auto row = it_row - cells.cbegin() + 1;
        if( row < 4){
            continue;
        }
        MazeOption option;
        for(auto it_col = it_row->cbegin(); it_col !=  it_row->cend();++it_col){
            auto col = it_col -  it_row->cbegin() + 1;
            switch (col) {
            case 1:  option.id = it_col->toInt() ; break;
            case 2:  option.text = it_col->toString() ; break;
            case 3:  option.activecond = it_col->toString() ; break;
            case 4:  option.disabledTips = it_col->toString() ; break;
            case 5:  option.linkStageId = it_col->toInt() ; break;
            case 6:  option.events = it_col->toStringList() ; break;
            case 7:  option.isonlyonce = it_col->toInt(); break;
            }
        }
        m_map.insert(option.id, std::shared_ptr<ExcelData>(new MazeOption(option)));
    }
}

