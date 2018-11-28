#include "stageblock.h"
#include "ui_stageblock.h"

StageBlock::StageBlock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StageBlock)
{
    ui->setupUi(this);
}

StageBlock::~StageBlock()
{
    delete ui;
}
