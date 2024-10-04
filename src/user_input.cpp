#include <iostream>
#include <thread>
#include <unistd.h>
#include <atomic>

std::thread input_thread;
std::atomic<bool> continue_execution(true);
std::atomic<char> buffer(0);

void readFromStdin()
{
    // continue, until user quits with 'q'
    while (continue_execution)
    {
        // fetch user input
        if (read(STDIN_FILENO, &buffer, 1) > 0)
        {
            if (buffer == 'q')
            {
                continue_execution = false;
            }
        }
    }
}

void start_user_input_thread()
{
    input_thread = std::thread(readFromStdin);
}
