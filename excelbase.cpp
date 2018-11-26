#include "excelbase.h"
#include <QFile>

#include <QList>
#include <QDebug>
#include <ActiveQt/QAxObject>

#define TC_FREE(x) \
{              \
    delete x;  \
    x = nullptr;  \
    }

//class ExcelBasePrivate
//{
//    Q_DECLARE_PUBLIC(ExcelBase)
//public:
//    explicit ExcelBasePrivate(ExcelBase *qptr);
//    ~ExcelBasePrivate();

//    void construct();
//    void destory();

//    ExcelBase *const q_ptr;

//#if defined(Q_OS_WIN)
//    QAxObject *excel;
//    QAxObject *books;
//    QAxObject *book;
//    QAxObject *sheets;
//    QAxObject *sheet;
//#endif
//    QString filename;
//    QString sheetName;
//};

//---------------------------------------------------------------------------------------------------


ExcelBase::ExcelBase(QObject *par)
    : QObject(par),
      sp_excel(nullptr),sp_books(nullptr),sp_book(nullptr),sp_sheets(nullptr), sp_sheet(nullptr)
{
}

ExcelBase::~ExcelBase()
{
    close();
}

void ExcelBase::connect()
{
    sp_excel = std::make_shared<QAxObject>(this);
    sp_excel->setControl("Excel.Application");
    sp_excel->setProperty("Visible", false);
    sp_excel->setProperty("DisplayAlerts", true);//显示警告看效果
    sp_books =std::shared_ptr<QAxObject>(sp_excel->querySubObject("Workbooks"));
}

void ExcelBase::disconnect()
{
    if (sp_book != nullptr && !sp_book->isNull())
    {
        sp_book->dynamicCall("Close(Boolean)", false);
    }
    if (sp_excel != nullptr && !sp_excel->isNull())
    {
        sp_excel->dynamicCall("Quit(void)");
    }
    sp_sheet.reset();
    sp_sheets.reset();
    sp_book.reset();
    sp_books.reset();
    sp_excel.reset();
    filename.clear();
    sheetName.clear();
}

bool ExcelBase::create(const QString &_filename)
{
    connect();
    if (sp_books != nullptr && !sp_books->isNull())
    {
        sp_books->dynamicCall("Add");
        sp_book = std::shared_ptr<QAxObject>(sp_excel->querySubObject("ActiveWorkBook"));
        sp_sheets = std::shared_ptr<QAxObject>(sp_book->querySubObject("WorkSheets"));
        currentSheet();
        filename = _filename;
        return true;
    }
    return false;
}

bool ExcelBase::open(const QString &filename)
{
    bool ret = false;
#if defined(Q_OS_WIN)
    connect();
    if (d->books != nullptr && !d->books->isNull())
    {
        d->book = d->books->querySubObject("Open(QString, QVariant)", filename, 0);
        ret = d->book != nullptr && !d->book->isNull();
        if (ret)
        {
            d->sheets = d->book->querySubObject("WorkSheets");
            d->filename = filename;
            currentSheet();
        }
    }
#else
    Q_UNUSED(filename)
#endif // Q_OS_WIN
    return ret;
}

void ExcelBase::saveAs(const QString &filename)
{
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->book != nullptr && !d->book->isNull())
    {
        d->filename = filename;
        QString strPath = d->filename;
        strPath = strPath.replace('/', '\\');
        qDebug() << strPath;
        d->book-> dynamicCall("SaveAs(const QString&)",strPath);
        //        d->book->dynamicCall("SaveAs(const QString&,int,const QString&,const QString&,bool,bool)", strPath, 56, QString(""), QString(""), false, false);
    }
#else
    Q_UNUSED(filename)
#endif // Q_OS_WIN
}
///
/// \brief 保存excel
///
void ExcelBase::save()
{
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->filename.isEmpty())
        return;
    saveAs(d->filename);
#else
    Q_UNUSED(filename)
#endif // Q_OS_WIN
}

void ExcelBase::close()
{
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    d->destory();
#endif // Q_OS_WIN
}

QStringList ExcelBase::sheetNames()
{
    QStringList ret;
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->sheets != nullptr && !d->sheets->isNull())
    {
        int sheetCount = d->sheets->property("Count").toInt();
        for (int i = 1; i <= sheetCount; i++)
        {
            QAxObject *sheet = d->sheets->querySubObject("Item(int)", i);
            if (nullptr == sheet || sheet->isNull())
                continue;
            ret.append(sheet->property("Name").toString());
            delete sheet;
        }
    }
#endif // Q_OS_WIN
    return ret;
}

QString ExcelBase::currentSheetName()
{
    Q_D(ExcelBase);
    return d->sheetName;
}

void ExcelBase::setVisible(bool value)
{
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->excel != nullptr && !d->excel->isNull())
    {
        d->excel->setProperty("Visible", value);
    }
#else
    Q_UNUSED(value)
#endif // Q_OS_WIN
}

void ExcelBase::setCaption(const QString &value)
{
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->excel != nullptr && !d->excel->isNull())
    {
        d->excel->setProperty("Caption", value);
    }
#else
    Q_UNUSED(value)
#endif // Q_OS_WIN
}

bool ExcelBase::addBook()
{
    bool ret = false;
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->excel != nullptr && !d->excel->isNull())
    {
        TC_FREE(d->sheet);
        TC_FREE(d->sheets);
        TC_FREE(d->book);
        TC_FREE(d->books);
        d->books = d->excel->querySubObject("WorkBooks");
        ret = d->books != nullptr && !d->books->isNull();
    }
#endif // Q_OS_WIN
    return ret;
}

bool ExcelBase::currentSheet()
{
    bool ret = false;
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    TC_FREE(d->sheet);
    if (d->excel != nullptr && !d->excel->isNull())
    {
        TC_FREE(d->sheet);
        d->sheet = d->excel->querySubObject("ActiveWorkBook");
        ret = d->sheet != nullptr && !d->sheet->isNull();
        d->sheetName = ret ? d->sheet->property("Name").toString() : "";
    }
#endif // Q_OS_WIN
    return ret;
}

bool ExcelBase::setCurrentSheet(int index)
{
    bool ret = false;
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->sheets != nullptr && !d->sheets->isNull())
    {
        TC_FREE(d->sheet);
        d->sheet = d->sheets->querySubObject("Item(int)", index);
        ret = d->sheet != nullptr && !d->sheet->isNull();
        if (ret)
        {
            d->sheet->dynamicCall("Activate(void)");
        }
        d->sheetName = ret ? d->sheet->property("Name").toString() : "";
    }
#else
    Q_UNUSED(index)
#endif // Q_OS_WIN
    return ret;
}

int ExcelBase::sheetCount()
{
    int ret = 0;
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->sheets != nullptr && !d->sheets->isNull())
    {
        ret = d->sheets->property("Count").toInt();
    }
#endif // Q_OS_WIN
    return ret;
}

void ExcelBase::cellFormat(int row, int col, const QString &format)
{
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->sheet != nullptr && !d->sheet->isNull())
    {
        QAxObject *range = d->sheet->querySubObject("Cells(int, int)", row, col);
        range->setProperty("NumberFormatLocal", format);
        delete range;
    }
#else
    Q_UNUSED(row)
    Q_UNUSED(col)
    Q_UNUSED(format)
#endif // Q_OS_WIN
}

void ExcelBase::cellAlign(int row, int col, Alignment hAlign, Alignment vAlign)
{
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->sheet != nullptr && !d->sheet->isNull())
    {
        QAxObject *range = d->sheet->querySubObject("Cells(int, int)", row, col);
        range->setProperty("HorizontalAlignment", hAlign);
        range->setProperty("VerticalAlignment", vAlign);
        delete range;
    }
#else
    Q_UNUSED(row)
    Q_UNUSED(col)
    Q_UNUSED(hAlign)
    Q_UNUSED(vAlign)
#endif // Q_OS_WIN
}

QVariant ExcelBase::read(int row, int col)
{
    QVariant ret;
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->sheet != nullptr && !d->sheet->isNull())
    {
        QAxObject *range = d->sheet->querySubObject("Cells(int, int)", row, col);
        //ret = range->property("Value");
        ret = range->dynamicCall("Value()");
        delete range;
    }
#else
    Q_UNUSED(row)
    Q_UNUSED(col)
#endif // Q_OS_WIN
    return ret;
}

void ExcelBase::write(int row, int col, const QVariant &value)
{
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->sheet != nullptr && !d->sheet->isNull())
    {
        QAxObject *range = d->sheet->querySubObject("Cells(int, int)", row, col);
        range->setProperty("Value", value);
        delete range;
    }
#else
    Q_UNUSED(row)
    Q_UNUSED(col)
    Q_UNUSED(value)
#endif // Q_OS_WIN
}

bool ExcelBase::usedRange(int &rowStart, int &colStart, int &rowEnd, int &colEnd)
{
    bool ret = false;
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->sheet != nullptr && !d->sheet->isNull())
    {
        QAxObject *urange = d->sheet->querySubObject("UsedRange");
        rowStart = urange->property("Row").toInt();
        colStart = urange->property("Column").toInt();
        rowEnd = urange->querySubObject("Rows")->property("Count").toInt();
        colEnd = urange->querySubObject("Columns")->property("Count").toInt();
        delete urange;
        ret = true;
    }
#else
    Q_UNUSED(rowStart)
    Q_UNUSED(colStart)
    Q_UNUSED(rowEnd)
    Q_UNUSED(colEnd)
#endif // Q_OS_WIN
    return ret;
}
///
/// \brief 读取整个sheet
/// \return
///
QVariant ExcelBase::readAll()
{
    QVariant var;
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (d->sheet != nullptr && !d->sheet->isNull())
    {
        QAxObject *usedRange = d->sheet->querySubObject("UsedRange");
        if (nullptr == usedRange || usedRange->isNull())
        {
            return var;
        }
        var = usedRange->dynamicCall("Value");
        delete usedRange;
    }
#endif
    return var;
}
///
/// \brief 读取整个sheet的数据，并放置到cells中
/// \param cells
///
void ExcelBase::readAll(QList<QList<QVariant>> &cells)
{
#if defined(Q_OS_WIN)
    castVariant2ListListVariant(readAll(), cells);
#else
    Q_UNUSED(cells);
#endif
}

///
/// \brief 写入一个表格内容
/// \param cells
/// \return 成功写入返回true
/// \see readAllSheet
///
bool ExcelBase::writeCurrentSheet(const QList<QList<QVariant>> &cells)
{
#if defined(Q_OS_WIN)
    Q_D(ExcelBase);
    if (cells.size() <= 0)
        return false;
    if (nullptr == d->sheet || d->sheet->isNull())
        return false;
    int row = cells.size();
    int col = cells.at(0).size();
    QString rangStr;
    convertToColName(col, rangStr);
    rangStr += QString::number(row);
    rangStr = "A1:" + rangStr;
    qDebug() << rangStr;
    QAxObject *range = d->sheet->querySubObject("Range(const QString&)", rangStr);
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
#else
    return true;
#endif
}
///
/// \brief 把列数转换为excel的字母列号
/// \param data 大于0的数
/// \return 字母列号，如1->A 26->Z 27 AA
///
void ExcelBase::convertToColName(int data, QString &res)
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
///
/// \brief 数字转换为26字母
///
/// 1->A 26->Z
/// \param data
/// \return
///
QString ExcelBase::to26AlphabetString(int data)
{
    QChar ch = data + 0x40; //A对应0x41
    return QString(ch);
}
///
/// \brief QList<QList<QVariant> >转换为QVariant
/// \param cells
/// \return
///
void ExcelBase::castListListVariant2Variant(const QList<QList<QVariant>> &cells, QVariant &res)
{
    QVariantList vars;
    const int rows = cells.size();
    for (int i = 0; i < rows; ++i)
    {
        vars.append(QVariant(cells[i]));
    }
    res = QVariant(vars);
}
///
/// \brief 把QVariant转为QList<QList<QVariant> >
/// \param var
/// \param res
///
void ExcelBase::castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant>> &res)
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
