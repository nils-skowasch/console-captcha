#include "user_input.h"
#include "meta.h"
#include <atomic>
#include <thread>
#include <unistd.h>

UserInput::UserInput()
    : continue_execution(true), buffer(0), inputThread(nullptr), meta(nullptr) {
}

void UserInput::readFromStdin() {
    // continue, until user quits with 'q'
    while (continue_execution) {
        // fetch user input
        if (read(STDIN_FILENO, &buffer, 1) > 0) {
            if (buffer == 'q') {
                continue_execution = false;
            }
        }
    }
}

void UserInput::startThread(Meta *m) {
    meta = m;
    inputThread = new std::thread([this] { readFromStdin(); });
}

void UserInput::joinThread() {
    inputThread->join();
    delete inputThread;
    inputThread = nullptr;
}

bool UserInput::isContinueExceution() {
    return continue_execution;
}
