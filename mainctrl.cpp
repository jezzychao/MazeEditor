#include "mainctrl.h"


MainCtrl::MainCtrl()
{

}

MainCtrl *MainCtrl::getInstance()
{
    static MainCtrl *p = new MainCtrl;
    return p;
}

void MainCtrl::start()
{
}
