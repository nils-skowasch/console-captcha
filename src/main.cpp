#include "console_output.cpp"
#include "meta.cpp"
#include "user_input_thread.cpp"
#include "win_condition_thread.cpp"

#define REFRESH_RATE_HZ 8

static void printResultMessage(const char *message) {
    clearLegend();
    moveCursor(0, FIELD_DIM_Y + 2);
    std::cout << message << std::endl;
    std::cout << "Would you be so kindly, and press any key to exit?" << std::endl;
}

int main() {
    // initialze Meta object, which serves as central data hub
    Meta meta;

    // start another thread to fetch user's keyboard input
    UserInputThread userInputThread;
    userInputThread.startThread(&meta);

    // start another thread to check the win condition regularly
    WinConditionThread winConditionThread;
    winConditionThread.startThread(&meta);

    // continue, until user quits
    while (meta.isContinueExecution()) {
        // check captcha win or failed condition reached
        if (winConditionThread.hasSucceededCaptcha()) {
            meta.stopExecution();
            printResultMessage("!! You seem to be a human, congratulations :D !!");
        } else if (winConditionThread.hasFailedCaptcha()) {
            meta.stopExecution();
            printResultMessage("!! Go away, stinky AI :[ !!");
        } else {
            // print all captcha output to console
            printOutput(&meta);

            // sleep a bit
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / REFRESH_RATE_HZ));
        }
    }

    // tease the user (or AI) ;)
    if (meta.hasUserSurrendered()) {
        printResultMessage("What a shame ...");
    }

    // wait for the userInputThread to finish
    userInputThread.joinThread();

    // wait for the winConditionThread to finish
    winConditionThread.joinThread();

    // clear and reset console for shutdown
    resetConsole();

    return 0;
}
