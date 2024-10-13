#include "user_input_thread.h"
#include "../obj/meta.h"
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
            if (buffer == 'q' && meta->isContinueExecution()) { // q: quit
                meta->userWantsToSurrender();
                read(STDIN_FILENO, &buffer, 1); // wait for the user final key event ('press any key')
            } else if (buffer == ' ') {
                meta->placeWire(); // place wire with SPACE key
                meta->setStdoutRefreshAll(true);
            } else if (!escape_mode && ((buffer >= COLOR_CHAR_START_UPPER && buffer <= COLOR_CHAR_END_UPPER) ||
                                        (buffer >= COLOR_CHAR_START_LOWER && buffer <= COLOR_CHAR_END_LOWER))) {
                meta->setSelectedColor(toColor(buffer)); // A-Da-d: select base color
                meta->setStdoutRefreshAll(true);
            } else if (buffer >= '1' && buffer <= '9') {
                int colorIndex = buffer - '0'; // 1-9: select mix color
                meta->setSelectedColorMix(static_cast<ColorMix>(colorIndex));
                meta->setStdoutRefreshAll(true);
            } else if (buffer == 27) { // ESC: ansi escape char
                escape_mode = true;
            } else if (escape_mode && buffer == 91) { // [: continue parsing the ansi escaped char
                continue;
            } else if (escape_mode && buffer == 'A') { // arrow up
                meta->moveCursorUp();
                escape_mode = false;
            } else if (escape_mode && buffer == 'B') { // arrow down
                meta->moveCursorDown();
                escape_mode = false;
            } else if (escape_mode && buffer == 'C') { // arrow right
                meta->moveCursorRight();
                escape_mode = false;
            } else if (escape_mode && buffer == 'D') { // arrow left
                meta->moveCursorLeft();
                escape_mode = false;
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
