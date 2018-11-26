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

    QVariant readAll();
    void readAll(QList<QList<QVariant>> &cells);
    bool writeCurrentSheet(const QList<QList<QVariant>> &cells);
    static void convertToColName(int data, QString &res);
    static QString to26AlphabetString(int data);
    static void castListListVariant2Variant(const QList<QList<QVariant>> &cells, QVariant &res);
    static void castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant>> &res);

private:

    QAxObject *sp_excel;
    QAxObject *sp_books;
    QAxObject *sp_book;
    QAxObject *sp_sheets;
    QAxObject *sp_sheet;
    QString filename;
    QString sheetName;
};

#endif // EXCELREADER_H
