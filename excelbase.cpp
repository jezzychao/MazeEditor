#include "excelbase.h"
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QDebug>
#include <ActiveQt/QAxObject>

ExcelBase::ExcelBase()
    :sp_excel(nullptr),sp_books(nullptr),sp_book(nullptr),sp_sheets(nullptr), sp_sheet(nullptr),isAlreadyClose(false)
{
}

ExcelBase::~ExcelBase()
{
    if(!isAlreadyClose){
        isAlreadyClose = true;
         close();
    }
}

bool ExcelBase::exists() const{
    QFileInfo fileInfo(filename);
    return fileInfo.exists();
}

bool ExcelBase::open(const QString &_filename)
{
    bool iscreated = false;
    filename = _filename;
    sp_excel = new QAxObject("Excel.Application");
    sp_excel->setProperty("Visible", false);
    sp_excel->setProperty("DisplayAlerts", true);//显示警告看效果
    sp_books =sp_excel->querySubObject("WorkBooks");
    if(!exists()){
        iscreated = true;
        sp_books->dynamicCall("Add");
    }
    sp_books->dynamicCall("Open(const QString&)", filename);
    sp_book = sp_excel->querySubObject("ActiveWorkBook");
    if(iscreated){
        saveAs(filename);
    }
    return iscreated;
}

bool ExcelBase::setSheet(int index)
{
    if(sp_book != nullptr && !sp_book->isNull()){
        sp_sheet = sp_book->querySubObject("WorkSheets(int)",index);
        return true;
    }
    return false;
}

QVariant ExcelBase::read(int row, int col)
{
    QVariant ret;
    if (sp_sheet != nullptr&& !sp_sheet->isNull())
    {
        QAxObject *range = sp_sheet->querySubObject("Cells(int, int)", row, col);
        ret = range->dynamicCall("Value()");
        delete range;
    }
    return ret;
}

void ExcelBase::write(int row, int col, const QVariant &value)
{
    if (sp_sheet != nullptr && !sp_sheet->isNull())
    {
        QAxObject *cell = sp_sheet->querySubObject("Cells(int, int)", row, col);
        if(cell != nullptr){
            cell->dynamicCall("setValue(const QVariant&)", value);
            delete cell;
        }
    }
}

void ExcelBase::write(int row, const QList<QVariant>&value)
{
    auto col = value.size();
    QString endPos;
    convertToColName(col, endPos);
    endPos += QString::number(row);
    QString beginPos("A"+QString::number(row));
    QString rangStr = beginPos + ":" + endPos;
    qDebug() << rangStr;
    QAxObject *range = sp_sheet->querySubObject("Range(const QString&)", rangStr);
    if (nullptr != range && !range->isNull())
    {
        QVariant var;
        castListVariant2Variant(value,var);
        range->setProperty("Value", var);
         delete range;
    }
}

void ExcelBase::saveAs(const QString &_filename)
{
    if (sp_book != nullptr&& !sp_book->isNull())
    {
        if (_filename.isEmpty()){
            qWarning("保存的文件路径是空的");
            return;
        }
        filename = _filename;
        QString strPath = filename;
        strPath = strPath.replace('/', '\\');
        qDebug() << strPath;
        sp_book-> dynamicCall("SaveAs(const QString&)",strPath);
    }
}

void ExcelBase::save()
{
    if (sp_book != nullptr&& !sp_book->isNull())
    {
        if (filename.isEmpty()){
            qWarning("保存的文件路径是空的");
            return;
        }
        sp_book-> dynamicCall("Save()");
    }
}

void ExcelBase::close()
{
    if (sp_book !=nullptr && !sp_book->isNull())
    {
        sp_book->dynamicCall("Close(Boolean)", false);
    }
    if (sp_excel !=nullptr&& !sp_excel->isNull())
    {
        sp_excel->dynamicCall("Quit(void)");
    }
    delete sp_sheet;
    delete sp_sheets;
    delete sp_book;
    delete sp_books;
    delete sp_excel;

    sp_sheet = nullptr;
    sp_sheets = nullptr;
    sp_book = nullptr;
    sp_books = nullptr;
    sp_excel = nullptr;
    filename.clear();
    sheetName.clear();
}

///
/// \brief 读取整个sheet的数据，并放置到cells中
/// \param cells
///
void ExcelBase::readAll(QList<QList<QVariant>> &cells)
{
    castVariant2ListListVariant(readAll(), cells);
}

QVariant ExcelBase::readAll()
{
    QVariant var;
    if (sp_sheet != nullptr && !sp_sheet->isNull())
    {
        QAxObject *usedRange = sp_sheet->querySubObject("UsedRange");
        if (nullptr == usedRange || usedRange->isNull())
        {
            return var;
        }
        var = usedRange->dynamicCall("Value");
        delete usedRange;
    }
    return var;
}

///
/// \brief 写入一个表格内容
/// \param cells
/// \return 成功写入返回true
/// \see readAllSheet
///
bool ExcelBase::writeCurrentSheet(const QList<QList<QVariant>> &cells)
{
    if (cells.size() <= 0)
        return false;
    if (nullptr == sp_sheet || sp_sheet->isNull())
        return false;
    int row = cells.size();
    int col = cells.at(0).size();
    QString rangStr;
    convertToColName(col, rangStr);
    rangStr += QString::number(row);
    rangStr = "A1:" + rangStr;
    qDebug() << rangStr;
    QAxObject *range = sp_sheet->querySubObject("Range(const QString&)", rangStr);
    if (nullptr == range || range->isNull())
    {
        return false;
    }
    bool succ = false;
    QVariant var;
    castListListVariant2Variant(cells, var);
    succ = range->setProperty("Value", var);
    delete range;
    return succ;
}

void convertToColName(int data, QString &res)
{
    Q_ASSERT(data > 0 && data < 65535);
    int tempData = data / 26;
    if (tempData > 0)
    {
        int mode = data % 26;
        convertToColName(mode, res);
        convertToColName(tempData, res);
    }
    else
    {
        res = (to26AlphabetString(data) + res);
    }
}

QString to26AlphabetString(int data)
{
    QChar ch = data + 0x40; //A对应0x41
    return QString(ch);
}

void castListVariant2Variant(const QList<QVariant> &cells, QVariant &res)
{
    QVariantList vars;
    for(auto it = cells.cbegin();it != cells.cend();++it){
        vars.append(*it);
    }
    res = QVariant(std::move(vars));
}

void castVariant2ListVariant(const QVariant &var, QList<QVariant> &res)
{
    QVariantList vars = var.toList();
    for(auto it = vars.cbegin();it != vars.cend();++it){
        res.push_back(*it);
    }
}

void castListListVariant2Variant(const QList<QList<QVariant>> &cells, QVariant &res)
{
    QVariantList vars;
    const int rows = cells.size();
    for (int i = 0; i < rows; ++i)
    {
        vars.append(QVariant(cells[i]));
    }
    res = QVariant(vars);
}

void castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant>> &res)
{
    QVariantList varRows = var.toList();
    if (varRows.isEmpty())
    {
        return;
    }
    const int rowCount = varRows.size();
    QVariantList rowData;
    for (int i = 0; i < rowCount; ++i)
    {
        rowData = varRows[i].toList();
        res.push_back(rowData);
    }
}
