#include "user_input.cpp"
#include "console_output.cpp"

#define REFRESH_RATE_HZ 2

int main()
{
    // start another thread to fetch user's keyboard input
    start_user_input_thread();

    // continue, until user quits
    while (continue_execution)
    {
        // draw the field
        draw_field();
        //console_playground();

        // sleep a bit
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / REFRESH_RATE_HZ));
    }

    // wait for the input_thread to finish
    input_thread.join();

    return 0;
}
