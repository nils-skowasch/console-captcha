#include "console_output.cpp"
#include "meta.cpp"
#include "user_input.cpp"

#define REFRESH_RATE_HZ 4

int main() {
    // initialze Meta object, which serves as central data hub
    Meta meta;

    // start another thread to fetch user's keyboard input
    UserInput userInput;
    userInput.startThread(&meta);

    // continue, until user quits
    while (userInput.isContinueExceution()) {
        // draw the field
        drawField(&meta);

        // sleep a bit
        std::this_thread::sleep_for(
            std::chrono::milliseconds(1000 / REFRESH_RATE_HZ));
    }

    // wait for the input_thread to finish
    userInput.joinThread();

    // clear and reset console for shutdown
    resetConsole();

    return 0;
}
