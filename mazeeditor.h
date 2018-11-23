#ifndef MAZEEDITOR_H
#define MAZEEDITOR_H

#include <QMainWindow>

namespace Ui {
class mazeeditor;
}

class mazeeditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit mazeeditor(QWidget *parent = nullptr);
    ~mazeeditor();

private slots:
    void on_actionview_triggered();

    void on_actionedit_triggered();

    void on_actionnew_triggered();

private:
    Ui::mazeeditor *ui;
};

#endif // MAZEEDITOR_H
