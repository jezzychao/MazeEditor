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
    item,
    localization
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

struct MazeOptionEx:public ExcelData
{
    int id;                                 //id
    int linkStageId;                //链接的stageId
    bool isonlyonce;             //是否只触发一次
    QString text;                   //选项对应的文本
    QString activecond;      //该选项激活的条件表达式
    QString disabledTips;   //当选项未激活时点击出现显示的文本
    QStringList events;       //选项会抛出的事件列表
};

struct MazeStageEx:public ExcelData
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

struct Localization: public ExcelData
{
    Localization()=default;
    Localization(const QString &_id,const QString &_cn)
        :id(_id),cn(_cn)
    {

    }
    QString id;
    QString cn;
    QString en;
    QString tw;
};

class ExcelRes : public ExcelBase
{
    typedef QMap<int,std::shared_ptr<ExcelData>>Container;
public:
    ExcelRes();

    ~ExcelRes() override = default;

    bool openExcel(EXCEL_FILES);

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
    virtual void clearAll() = 0;
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
    void clearAll() override {}
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
        return false;
    }
    QList<QVariant> toExcel(const ExcelData &)override{
        qFatal("不能对item.xlsx进行写入操作");
    }
    std::tuple<int,std::shared_ptr<ExcelData>> toObject(const QList<QVariant>&) override;
    void clearAll() override {}
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
    void clearAll() override {}
};

class Exlocalization: public ExcelRes
{
public:
    Exlocalization() = default;
    ~Exlocalization() override  =default;
    bool insert(const Localization &);
    bool erase(const Localization &);
    QString getId(const QString &cn);
    void removeUnused(const QStringList &usedIds);

protected:
    bool initFields() override{
        qFatal("不能对item.xlsx修改字段");
    }
    QList<QVariant> toExcel(const ExcelData &)override;
    std::tuple<int,std::shared_ptr<ExcelData>> toObject(const QList<QVariant>&) override;
     void clearAll() override;
private:
    std::tuple<bool,QMap<int,std::shared_ptr<ExcelData>>::iterator> isAlreadyExist(const Localization&);
    QString genNewId();
};

#endif // EXCELRES_H
