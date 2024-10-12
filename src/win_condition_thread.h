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
    bool followWire(WireStart *wireStart);
    bool followWire(Merger *merger);
    bool followWire(std::vector<int> *seenWireIds, bool merged, int lastWireX, int lastWireY, int currentWireX,
                    int currentWireY);
    bool followWireNext(std::vector<int> *seenWireIds, bool merged, Wire *currentWire, int currentWireX,
                        int currentWireY, int nextWireX, int nextWireY);

  public:
    WinConditionThread();
    ~WinConditionThread();
    void startThread(Meta *m);
    void joinThread();
    bool hasSucceededCaptcha();
    bool hasFailedCaptcha();
};

#endif