#ifndef win_condition_h
#define win_condition_h

#include "meta.h"
#include <thread>

class WinConditionThread {
  private:
    std::thread *winConditionThread;
    Meta *meta;
    bool captchaSucceeded = false;
    bool captchaFailed = false;
    void handleCaptcha();
    bool followWire(int lastWireX, int lastWireY, int currentWireX, int currentWireY);

  public:
    WinConditionThread();
    ~WinConditionThread();
    void startThread(Meta *m);
    void joinThread();
    bool hasSucceededCaptcha();
    bool hasFailedCaptcha();
};

#endif