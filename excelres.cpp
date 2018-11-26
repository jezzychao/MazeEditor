#include "excelres.h"
#include "globalfunc.h"
#include <QFile>
#include <QMessageBox>

ExcelRes::ExcelRes(const QString &file)
   :ExcelBase (nullptr),
   filepath(myfunc::getExcelFilesPath() + "/" + file)
{

}

bool ExcelRes::createExcel(const QVector<std::tuple<QString,FIELD_TYPE>> &fields )
{
    if(exists()){
        QMessageBox::information(nullptr, "警告","该excel文件已经存在，无法创建" );
        return false;
    }
    if(create(filepath)){
        setCurrentSheet(1);
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
            field2col.insert(fieldname, col);
        }
        save();
        return true;
    }
    return false;
}

 bool ExcelRes::exists() const
 {
     QFile file(filepath);
     return file.exists();
 }

ExcelData& ExcelRes::at(int id)
{
    auto it = m_map.find(id);
    if(it == m_map.end()){
        qFatal("excel: %s 中不存在 id: %d 的数据",filepath.toStdString().c_str(), id);
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

ExcelMazeOption::ExcelMazeOption(const QString &filename)
    :ExcelRes (filename)
{
}

ExcelMazeOption::~ExcelMazeOption()
{

}

bool ExcelMazeOption::saveExcel()
{
    const auto &container = getContainer();
    int row = 3;
    for(auto it = container.cbegin();it != container.cend();++it){
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

void ExcelMazeOption::loadExcel()
{
      setCurrentSheet(1);
     auto &container = getContainer();
    QList<QList<QVariant>> cells;
     readAll(cells);
     for(auto it_row = cells.cbegin(); it_row != cells.cend();++it_row){
         auto row = it_row - cells.cbegin() + 1;
         if( row < 4){
             continue;
         }
         for(auto it_col = it_row->cbegin(); it_col !=  it_row->cend();++it_col){
                MazeOption option;
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
                container.insert(option.id, std::shared_ptr<ExcelData>(new MazeOption(option)));
         }
     }
}

