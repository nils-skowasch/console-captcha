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

        // follow wireStart0 and wireStart1 to Merger
        if (followWire(meta->getWireStart0()) && followWire(meta->getWireStart1())) {
            // merge wire colors
            Color color0 = meta->getWireStart0()->getWire()->getColor();
            Color color1 = meta->getWireStart1()->getWire()->getColor();
            ColorMix merged = merge(color0, color1);
            if (merged == ColorMix::None) {
                break; // undefined color combination, assume captcha failed
            }

            // continue following from Merger
            if (followWire(meta->getMerger())) {
                captchaSucceeded = true;
                break;
            }
        }

        // sleep one second
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

bool WinConditionThread::followWire(WireStart *wireStart) {
    std::vector<int> seenWireIds(USER_ACTIONS);
    return followWire(&seenWireIds, false, wireStart->getX(), wireStart->getY(), wireStart->getX(), wireStart->getY());
}

bool WinConditionThread::followWire(Merger *merger) {
    // top
    {
        int x = merger->getX();
        int y = merger->getY() - 1;
        std::vector<int> seenWireIds(USER_ACTIONS);
        if (followWire(&seenWireIds, true, x, y, x, y)) {
            return true;
        }
    }
    // right
    {
        int x = merger->getX() + 1;
        int y = merger->getY();
        std::vector<int> seenWireIds(USER_ACTIONS);
        if (followWire(&seenWireIds, true, x, y, x, y)) {
            return true;
        }
    }
    // bottom
    {
        int x = merger->getX();
        int y = merger->getY() + 1;
        std::vector<int> seenWireIds(USER_ACTIONS);
        if (followWire(&seenWireIds, true, x, y, x, y)) {
            return true;
        }
    }
    // left
    {
        int x = merger->getX() - 1;
        int y = merger->getY();
        std::vector<int> seenWireIds(USER_ACTIONS);
        if (followWire(&seenWireIds, true, x, y, x, y)) {
            return true;
        }
    }
    return false;
}

bool WinConditionThread::followWire(std::vector<int> *seenWireIds, bool merged, int lastWireX, int lastWireY,
                                    int currentWireX, int currentWireY) {
    // do not continue, if the given x/y coordinates are no valid game field coordinates
    if (!meta->isInGameFieldDimension(currentWireX, currentWireY)) {
        return false;
    }

    // get node from game field
    Node *node = meta->getGameFieldNodeAt(currentWireX, currentWireY);
    if (node == nullptr) {
        return false; // current position points to an empty field
    }

    // continue if the current element is a wire
    if (Wire *currentWire = dynamic_cast<Wire *>(node)) {
        seenWireIds->push_back(currentWire->getId()); // remember current wireId to prevent loops

        // check top
        if (followWireNext(seenWireIds, merged, currentWire, currentWireX, currentWireY, currentWireX,
                           currentWireY - 1)) {
            return true;
        }

        // check right
        if (followWireNext(seenWireIds, merged, currentWire, currentWireX, currentWireY, currentWireX + 1,
                           currentWireY)) {
            return true;
        }

        // check bottom
        if (followWireNext(seenWireIds, merged, currentWire, currentWireX, currentWireY, currentWireX,
                           currentWireY + 1)) {
            return true;
        }

        // check left
        if (followWireNext(seenWireIds, merged, currentWire, currentWireX, currentWireY, currentWireX - 1,
                           currentWireY)) {
            return true;
        }
    }

    return false; // found nothing, the captcha has not been solved yet
}

bool WinConditionThread::followWireNext(std::vector<int> *seenWireIds, bool merged, Wire *currentWire, int currentWireX,
                                        int currentWireY, int nextWireX, int nextWireY) {
    if (meta->isInGameFieldDimension(nextWireX, nextWireY)) {
        Node *nextNode = meta->getGameFieldNodeAt(nextWireX, nextWireY);
        if (nextNode != nullptr) { // found a node at the next location

            // check whether the wire reached it's final destination
            if (merged) {
                // check whether the nextNode is the terminus location
                if (dynamic_cast<Term *>(nextNode)) {
                    return true; // terminus location found
                }
            } else {
                // check whether the nextNode is the merger location
                if (dynamic_cast<Merger *>(nextNode)) {
                    return true; // merger location found
                }
            }

            // consider the nextNode being a wire
            if (Wire *nextWire = dynamic_cast<Wire *>(nextNode)) {
                // do not follow backwards or get trapped in loops
                if (!isVectorContainsValue(seenWireIds, nextWire->getId())) {
                    // follow only, if the color is matching
                    if (currentWire->hasMatchingRGB(nextWire->getCharacterRGB())) {
                        // all fine, follow the wire
                        return followWire(
                            seenWireIds, merged, currentWireX, currentWireY, nextWireX,
                            nextWireY); // follow the wire from here, if this is a dead end, try the other directions
                    }
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
