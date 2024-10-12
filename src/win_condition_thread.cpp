#include "win_condition_thread.h"
#include <algorithm>
#include <chrono>

static bool isVectorContainsValue(std::vector<int> *v, int value) {
    return std::find(v->begin(), v->end(), value) != v->end();
}

void WinConditionThread::handleCaptcha() {
    while (meta->isContinueExecution()) {
        // handle captcha failed state
        if (!meta->hasActionsLeft()) {
            captchaFailed = true;
            return;
        }

        // follow startWire0
        if (!captchaSucceeded) {
            captchaSucceeded = followWire(meta->getStartWire0());
        }

        // follow startWire1
        if (!captchaSucceeded) {
            captchaSucceeded = followWire(meta->getStartWire1());
        }

        // sleep one second
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

bool WinConditionThread::followWire(std::vector<int> *seenWireIds, int lastWireX, int lastWireY, int currentWireX,
                                    int currentWireY) {
    Node *node = meta->getGameFieldNodeAt(currentWireX, currentWireY);
    if (node == nullptr) {
        return false; // current position points to an empty game field
    }

    // this one is a wire
    Wire *currentWire = (Wire *)node;
    seenWireIds->push_back(currentWire->getId()); // remember current wireId to prevent loops

    // check top
    if (followWireNext(seenWireIds, currentWire, currentWireX, currentWireY, currentWireX, currentWireY - 1)) {
        return true;
    }

    // check right
    if (followWireNext(seenWireIds, currentWire, currentWireX, currentWireY, currentWireX + 1, currentWireY)) {
        return true;
    }

    // check bottom
    if (followWireNext(seenWireIds, currentWire, currentWireX, currentWireY, currentWireX, currentWireY + 1)) {
        return true;
    }

    // check left
    if (followWireNext(seenWireIds, currentWire, currentWireX, currentWireY, currentWireX - 1, currentWireY)) {
        return true;
    }

    return false; // found nothing, the captcha has not been solved yet
}

bool WinConditionThread::followWire(StartWire *startWire) {
    std::vector<int> seenWireIds(USER_ACTIONS);
    return followWire(&seenWireIds, startWire->getX(), startWire->getY(), startWire->getX(), startWire->getY());
}

bool WinConditionThread::followWireNext(std::vector<int> *seenWireIds, Wire *currentWire, int currentWireX,
                                        int currentWireY, int nextWireX, int nextWireY) {
    if (nextWireX >= 0 && nextWireX < (FIELD_DIM_X - 2) && nextWireY >= 0 && nextWireY < (FIELD_DIM_Y - 2)) {
        Node *nextNode = meta->getGameFieldNodeAt(nextWireX, nextWireY);
        if (nextNode != nullptr) { // found a node at the next location
            // check wether the nextNode is the terminus location
            if (dynamic_cast<Term *>(nextNode)) {
                return true; // terminus location found
            }

            // consider the nextNode being a wire
            Wire *nextWire = (Wire *)nextNode;

            // do not follow backwards or get trapped in loops
            if (!isVectorContainsValue(seenWireIds, nextWire->getId())) {
                // follow only, if the color is not matching
                if (currentWire->hasMatchingRGB(nextWire->getCharacterRGB())) {
                    // all fine, follow the wire
                    return followWire(
                        seenWireIds, currentWireX, currentWireY, nextWireX,
                        nextWireY); // follow the wire from here, if this is a dead end, try the other directions
                }
            }
        }
    }
    return false;
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
