#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>
#include "console.cpp"

#define REFRESH_RATE_HZ 2

int main()
{
    bool run = true;
    char key;
    while (run)
    {
        // draw the field
        draw_field();

        // fetch user input
        if (read(STDIN_FILENO, &key, 1) > 0) // unfortunately blocking
        {
            if (key == 'q')
            {
                run = false;
            }
        }

        // sleep a bit
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / REFRESH_RATE_HZ));
    }
    return 0;
}
