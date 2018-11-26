#ifndef EXCELRES_H
#define EXCELRES_H

#include "excelbase.h"
#include <tuple>
#include <QVector>
#include <QString>
#include <memory>
#include <QMap>

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

class ExcelRes : public ExcelBase
{
    typedef QMap<int,std::shared_ptr<ExcelData>>Container;
public:
    ExcelRes(const QString &);

    ExcelRes(ExcelRes &) = delete;

    ExcelRes &operator=(ExcelRes &) = delete;

    ~ExcelRes() = default;

     ///@brief 创建excel,初始化字段头
    bool createExcel(const QVector<std::tuple<QString,FIELD_TYPE>> &);

    virtual bool saveExcel() = 0;

    virtual void loadExcel() = 0;

    bool exists() const;

    ExcelData& at(int id);

    ExcelData get(int id);

    bool set(int id, ExcelData *);

protected:
   const QString &getPath() const{return filepath;}
   Container &getContainer() {return m_map;}

 private:
    QMap<QString, int> field2col;//字段所在的列号
   Container m_map;
   QString filepath;
};

class ExcelMazeOption: public ExcelRes
{
public:
    ExcelMazeOption(const QString &);

    ~ExcelMazeOption() override;

     bool saveExcel() override;

     void loadExcel() override;
};

#endif // EXCELRES_H
