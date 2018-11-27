#ifndef EXCELREADER_H
#define EXCELREADER_H
#include <QObject>
#include <QAxObject>
#include <QString>
#include <QStringList>
#include <QVariant> 

class ExcelBase
{
public:
    ExcelBase();
    ExcelBase(const ExcelBase &) = delete;
    ExcelBase &operator=(const ExcelBase &) = delete;
    virtual ~ExcelBase();
protected:
    bool exists() const;

    /// @brief 打开一个Microsoft Excel文件，不存在则会创建一个
    bool open(const QString &filename = QString());

    /// @brief 另存Microsoft Excel文件
    void saveAs(const QString &filename);

    void save();

    /// @brief 关闭Microsoft Excel文件
    void close();



    QString getFullPath()const{return filename;}

    /// @brief 读取单元格 Sheet 的内容
    /// @param [in] row 行号，从 1 开始
    /// @param [in] col 列号，从 1 开始
    /// @return 返回指定单元格的内容
    QVariant read(int row, int col);

    /// @brief 读取单元格 Sheet 的内容
    /// @param [in] row 行号，从 1 开始
    /// @param [in] col 列号，从 1 开始
    /// @return 返回指定单元格的内容
    inline QVariant cell(int row, int col) { return read(row, col); }

    /// @brief 写入单元格 Sheet 的内容
    /// @param [in] row 行号，从 1 开始
    /// @param [in] col 列号，从 1 开始
    /// @param [in] value 准备写入的内容
    void write(int row, int col, const QVariant &value);

     void write(int row, const QList<QVariant>&value);

    QVariant readAll();
    void readAll(QList<QList<QVariant>> &cells);
    bool writeCurrentSheet(const QList<QList<QVariant>> &cells);
private:
    bool isAlreadyClose;
    QAxObject *sp_excel;
    QAxObject *sp_books;
    QAxObject *sp_book;
    QAxObject *sp_sheets;
    QAxObject *sp_sheet;
    QString filename;
    QString sheetName;
};

///
/// \brief 把列数转换为excel的字母列号
/// \param data 大于0的数
/// \return 字母列号，如1->A 26->Z 27 AA
///
void convertToColName(int data, QString &res);

QString to26AlphabetString(int data);

///
/// \brief QList<QVariant >转换为QVariant
/// \param cells
/// \return
///
void castListVariant2Variant(const QList<QVariant> &cells, QVariant &res);

///
/// \brief 把QVariant转为QList<QVariant >
/// \param var
/// \param res
///
void castVariant2ListVariant(const QVariant &var, QList<QVariant> &res);

///
/// \brief QList<QList<QVariant> >转换为QVariant
/// \param cells
/// \return
///
 void castListListVariant2Variant(const QList<QList<QVariant>> &cells, QVariant &res);

 ///
 /// \brief 把QVariant转为QList<QList<QVariant> >
 /// \param var
 /// \param res
 ///
 void castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant>> &res);

#endif // EXCELREADER_H
