#ifndef EXCELRES_H
#define EXCELRES_H

#include "excelbase.h"
#include <tuple>
#include <QVector>
#include <QString>
#include <memory>
#include <QMap>

enum class EXCEL_FILES{
    mazeStage,
    mazeOption,
    pot,
    item
};

enum class FIELD_TYPE{
    NON = 0,
    INT = 1,
    STR =2,
    LIST = 3
};

struct ExcelData
{

};

struct MazeOption:public ExcelData
{
    int id;                                 //id
    int linkStageId;                //链接的stageId
    bool isonlyonce;             //是否只触发一次
    QString text;                   //选项对应的文本
    QString activecond;      //该选项激活的条件表达式
    QString disabledTips;   //当选项未激活时点击出现显示的文本
    QStringList events;       //选项会抛出的事件列表
};

struct MazeStage:public ExcelData
{
    int id;
    int type;
    int mappingId;
    int entryStoryId;
    QString name;
    QString sprite;
    QString desc;
    QVector<int> optionIds;
};

struct Item:public ExcelData
{
    int id;
    int type;
    QString remark;
};

struct Pot:public ExcelData
{
    int id;
    QString name;
    QString desc;
    int type;
    int x;
    int y;
    QString icon;
    QString icon1;
    QStringList unlock;
    int cond;
    QStringList stage;
    int target;
    QStringList weight;
};

class ExcelRes : public ExcelBase
{
    typedef QMap<int,std::shared_ptr<ExcelData>>Container;
public:
    ExcelRes();

    ~ExcelRes() override = default;

    bool openExcel(EXCEL_FILES,bool isBackup = false);

    bool saveExcel();

    void loadExcel();

    void closeExcel();

    ExcelData& at(int id);

    ExcelData get(int id);

    bool set(int id, ExcelData *);

protected:
    virtual bool initFields() = 0;
    virtual  QList<QVariant> toExcel(const ExcelData &) = 0;
    virtual  std::tuple<int,std::shared_ptr<ExcelData>> toObject(const QList<QVariant>&) = 0;
    ///@brief 初始化字段头
   bool addFields(const QVector<std::tuple<QString,FIELD_TYPE>> &);

   Container &getContainer() {return m_map;}

 private:
   Container m_map;
};

class ExMazeOption: public ExcelRes
{
public:
    ExMazeOption() = default;
    ~ExMazeOption() override  =default;
protected:
     bool initFields() override;
      QList<QVariant> toExcel(const ExcelData &)override;
      std::tuple<int,std::shared_ptr<ExcelData>> toObject(const QList<QVariant>&) override;
};

class ExItem: public ExcelRes
{
public:
    ExItem() = default;
    ~ExItem() override  =default;
    QVector<Item> getValidTickets();
protected:
    bool initFields() override{
        qFatal("不能对item.xlsx修改字段");
    }
     QList<QVariant> toExcel(const ExcelData &)override{
        qFatal("不能对item.xlsx进行写入操作");
     }
      std::tuple<int,std::shared_ptr<ExcelData>> toObject(const QList<QVariant>&) override;
};

class ExPot: public ExcelRes
{
    public:
    ExPot() = default;
    ~ExPot() override  =default;
protected:
    bool initFields() override{
        qFatal("不能对item.xlsx修改字段");
    }
     QList<QVariant> toExcel(const ExcelData &)override;
      std::tuple<int,std::shared_ptr<ExcelData>> toObject(const QList<QVariant>&) override;
};

#endif // EXCELRES_H
