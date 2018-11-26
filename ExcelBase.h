#ifndef EXCELREADER_H
#define EXCELREADER_H
#include <QObject>
#include <QAxObject>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <memory>

class ExcelBase : public QObject
{
public:
    ExcelBase(QObject *par = nullptr);
    ExcelBase(const ExcelBase &) = delete;
    ExcelBase &operator=(const ExcelBase &) = delete;
    virtual ~ExcelBase();

    bool exists() const;

    /// @brief 创建一个Microsoft Excel文件
    bool create(const QString &filename = QString());
    /// @brief 打开一个Microsoft Excel文件
    bool open(const QString &filename = QString());
    /// @brief 另存Microsoft Excel文件
    void saveAs(const QString &filename);
    void save();
    /// @brief 关闭Microsoft Excel文件
    void close();
    /// @brief 设置当前打开的 Excel 是否可见
    void setVisible(bool value);
    /// @brief 设置 Excel 文档的标题
    void setCaption(const QString &value);

    /// @brief 新建一本 Excel 文档
    bool addBook();

    /// @brief 返回当前 Excel 的 Sheet 数量
    int sheetCount();

    /// @brief 返回当前打开的 Excel 全部 Sheet 名
    QStringList sheetNames();

    /// @brief 返回当前 Sheet.
    bool currentSheet();

    /// @brief 设置并指定当前 Sheet.
    /// @param [in] 当前 Sheet 索引，从 1 开始
    bool setCurrentSheet(int index);

    /// @brief 当前打开的 Excel 的 Sheet 名
    QString currentSheetName();

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

    /// @brief 获取有效区域信息
    /// @see rowStart() const
    /// @see rowEnd() const
    /// @see colStart() const
    /// @see colEnd() const
    bool usedRange(int &rowStart, int &colStart, int &rowEnd, int &colEnd);

    QVariant readAll();
    void readAll(QList<QList<QVariant>> &cells);
    bool writeCurrentSheet(const QList<QList<QVariant>> &cells);

    static void convertToColName(int data, QString &res);
    static QString to26AlphabetString(int data);
    static void castListListVariant2Variant(const QList<QList<QVariant>> &cells, QVariant &res);
    static void castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant>> &res);

private:
    void connect();
    void disconnect();

    std::shared_ptr<QAxObject> sp_excel;
    std::shared_ptr<QAxObject> sp_books;
    std::shared_ptr<QAxObject> sp_book;
    std::shared_ptr<QAxObject> sp_sheets;
    std::shared_ptr<QAxObject> sp_sheet;
    QString filename;
    QString sheetName;
};

#endif // EXCELREADER_H
