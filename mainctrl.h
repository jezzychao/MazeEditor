#ifndef MAINCTRL_H
#define MAINCTRL_H

#include <memory>

class ResView;
class ResConfig;

class MainCtrl
{
public:
    static MainCtrl *getInstance();

    void start();

private:
    MainCtrl();
    std::shared_ptr<ResConfig> spc;
    std::shared_ptr<ResView> spv;
};

#endif // MAINCTRL_H
