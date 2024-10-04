#ifndef user_input_h
#define user_input_h

#include "meta.h"
#include <atomic>
#include <thread>
#include <unistd.h>

class UserInput {
  private:
    std::atomic<bool> continue_execution;
    std::atomic<char> buffer;
    std::thread *inputThread;
    Meta *meta;
    void readFromStdin();

  public:
    UserInput();
    ~UserInput();
    void startThread(Meta *m);
    void joinThread();
    bool isContinueExceution();
};

#endif