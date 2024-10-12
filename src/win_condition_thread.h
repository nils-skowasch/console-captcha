#ifndef win_condition_h
#define win_condition_h

#include "meta.h"
#include <thread>
#include <vector>

class WinConditionThread {
  private:
    std::thread *winConditionThread;
    Meta *meta;
    bool captchaSucceeded = false;
    bool captchaFailed = false;
    void handleCaptcha();
    bool followWire(std::vector<int> *seenWireIds, int lastWireX, int lastWireY, int currentWireX, int currentWireY);
    bool followWire(StartWire *startWire);
    bool followWireNext(std::vector<int> *seenWireIds, int currentWireX, int currentWireY, int nextWireX, int nextWireY);

  public:
    WinConditionThread();
    ~WinConditionThread();
    void startThread(Meta *m);
    void joinThread();
    bool hasSucceededCaptcha();
    bool hasFailedCaptcha();
};

#endif