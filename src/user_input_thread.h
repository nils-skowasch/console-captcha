#ifndef user_input_thread_h
#define user_input_thread_h

#include "meta.h"
#include <atomic>
#include <thread>
#include <unistd.h>

class UserInputThread {
  private:
    std::atomic<char> buffer;
    std::thread *inputThread;
    Meta *meta;
    bool isColorChar(unsigned char character);
    bool isColorIndex(unsigned char character);
    void readFromStdin();

  public:
    UserInputThread();
    ~UserInputThread();
    void startThread(Meta *m);
    void joinThread();
};

#endif