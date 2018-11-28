#ifndef STAGEBLOCK_H
#define STAGEBLOCK_H

#include <QWidget>

namespace Ui {
class StageBlock;
}

class StageBlock : public QWidget
{
    Q_OBJECT

public:
    explicit StageBlock(QWidget *parent = nullptr);
    ~StageBlock();

private:
    Ui::StageBlock *ui;
};

#endif // STAGEBLOCK_H
