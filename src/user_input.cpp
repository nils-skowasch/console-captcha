#include "user_input.h"
#include "meta.h"
#include <atomic>
#include <termios.h>
#include <thread>
#include <unistd.h>

/**
 * Disable canonical mode to receive every key pressed event directly.
 */
static void disableCanonicalMode() {
    // disable terminal canonical mode
    struct termios term;

    // get current terminal attributes
    tcgetattr(STDIN_FILENO, &term);

    // disable canonical mode and echo
    term.c_lflag &= ~(ICANON | ECHO);

    // set the new terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

/**
 * Enable the canonical mode to leave the terminal in a usable way.
 */
static void enableCanonicalMode() {
    struct termios term;

    // Get current terminal attributes
    tcgetattr(STDIN_FILENO, &term);

    // Restore canonical mode and echo
    term.c_lflag |= (ICANON | ECHO);

    // Set the restored terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

UserInput::UserInput()
    : continue_execution(true), buffer(0), inputThread(nullptr), meta(nullptr) {
    disableCanonicalMode();
}

UserInput::~UserInput() {
    enableCanonicalMode();
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
