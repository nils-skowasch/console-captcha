#include "user_input_thread.h"
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

UserInputThread::UserInputThread() : buffer(0), inputThread(nullptr), meta(nullptr) {
    disableCanonicalMode();
}

UserInputThread::~UserInputThread() {
    enableCanonicalMode();
}

void UserInputThread::readFromStdin() {
    bool escape_mode = false; // used to switch into "ansi-escape-char-parsing-mode" ;)

    // continue, until user quits with 'q'
    while (meta->isContinueExecution()) {
        // fetch user input
        if (read(STDIN_FILENO, &buffer, 1) > 0) {
            if (buffer == 'q') {
                meta->stopExecution();
            } else if (buffer == ' ') {
                meta->placeWire();
            } else if (buffer == 27) { // ansi escape char
                escape_mode = true;
            } else if (escape_mode && buffer == 91) {
                continue;                              // continue parsing the ansi escaped char
            } else if (escape_mode && buffer == 'A') { // arrow up
                meta->moveCursorUp();
            } else if (escape_mode && buffer == 'B') { // arrow down
                meta->moveCursorDown();
            } else if (escape_mode && buffer == 'C') { // arrow right
                meta->moveCursorRight();
            } else if (escape_mode && buffer == 'D') { // arrow left
                meta->moveCursorLeft();
            } else {
                escape_mode = false;
            }
        }
    }
}

void UserInputThread::startThread(Meta *m) {
    meta = m;
    inputThread = new std::thread([this] { readFromStdin(); });
}

void UserInputThread::joinThread() {
    inputThread->join();
    delete inputThread;
    inputThread = nullptr;
}
