#include "win_condition_thread.h"
#include <chrono>

void WinConditionThread::handleCaptcha() {
    while (meta->isContinueExecution()) {
        // handle captcha failed state
        if (!meta->hasActionsLeft()) {
            captchaFailed = true;
            return;
        }

        // handle captcha succeeded state
        int wireX = -1, wireY = -1;
        // find wire start
        for (int y = 0; y < FIELD_DIM_Y - 2; y++) {
            if (meta->getGameFieldCharAt(0, y) == WIRE_CHAR) {
                wireX = 0;
                wireY = y;
                break;
            }
        }

        // follow the wire
        if (wireX != -1) {
            captchaSucceeded = followWire(wireX, wireY, wireX, wireY);
        }

        // sleep one second
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

bool WinConditionThread::followWire(int lastWireX, int lastWireY, int currentWireX, int currentWireY) {
    // check terminus destination reached
    if (meta->getGameFieldCharAt(currentWireX, currentWireY) == TERM_CHAR) {
        return true;
    }

    int nextWireX, nextWireY;
    unsigned char nextChar;

    // check top
    nextWireX = currentWireX;
    nextWireY = currentWireY - 1;
    if (nextWireX != lastWireX || nextWireY != lastWireY) { // do not follow backwards
        nextChar = meta->getGameFieldCharAt(nextWireX, nextWireY);
        if (nextChar == WIRE_CHAR || nextChar == TERM_CHAR) {
            return followWire(currentWireX, currentWireY, nextWireX,
                              nextWireY); // found the next wire part on the top side
        }
    }

    // check right
    nextWireX = currentWireX + 1;
    nextWireY = currentWireY;
    if (nextWireX != lastWireX || nextWireY != lastWireY) { // do not follow backwards
        nextChar = meta->getGameFieldCharAt(nextWireX, nextWireY);
        if (nextChar == WIRE_CHAR || nextChar == TERM_CHAR) {
            return followWire(currentWireX, currentWireY, nextWireX,
                              nextWireY); // found the next wire part on the right side
        }
    }

    // check bottom
    nextWireX = currentWireX;
    nextWireY = currentWireY + 1;
    if (nextWireX != lastWireX || nextWireY != lastWireY) { // do not follow backwards
        nextChar = meta->getGameFieldCharAt(nextWireX, nextWireY);
        if (nextChar == WIRE_CHAR || nextChar == TERM_CHAR) {
            return followWire(currentWireX, currentWireY, nextWireX,
                              nextWireY); // found the next wire part on the bottom side
        }
    }

    // check left
    nextWireX = currentWireX - 1;
    nextWireY = currentWireY;
    if (nextWireX != lastWireX || nextWireY != lastWireY) { // do not follow backwards
        nextChar = meta->getGameFieldCharAt(nextWireX, nextWireY);
        if (nextChar == WIRE_CHAR || nextChar == TERM_CHAR) {
            return followWire(currentWireX, currentWireY, nextWireX,
                              nextWireY); // found the next wire part on the left side
        }
    }

    return false; // found nothing, the captcha has not been solved yet
}

WinConditionThread::WinConditionThread() : winConditionThread(nullptr), meta(nullptr) {
}

WinConditionThread::~WinConditionThread() {
}

void WinConditionThread::startThread(Meta *m) {
    meta = m;
    winConditionThread = new std::thread([this] { handleCaptcha(); });
}

void WinConditionThread::joinThread() {
    winConditionThread->join();
    delete winConditionThread;
    winConditionThread = nullptr;
}

bool WinConditionThread::hasSucceededCaptcha() {
    return captchaSucceeded;
}

bool WinConditionThread::hasFailedCaptcha() {
    return captchaFailed;
}
