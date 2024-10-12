#include "color_table.h"
#include <cstdlib>
#include <stdexcept>

RGB getRGB(Color color) {
    switch (color) {
    case Color::RED:
        return {255, 0, 0};
    case Color::GREEN:
        return {0, 255, 0};
    case Color::BLUE:
        return {0, 0, 255};
    case Color::YELLOW:
        return {100, 95, 5};
    default:
        throw std::runtime_error("Unexpected Color enum value! Guess you lost the captcha^^");
    }
}

RGB getRGB(ColorMix colorMix) {
    switch (colorMix) {
    case ColorMix::None:
        return {42, 42, 42};
    case ColorMix::RED:
        return getRGB(Color::RED);
    case ColorMix::BROWN:
        return {61, 40, 0};
    case ColorMix::VIOLET:
        return {62, 0, 64};
    case ColorMix::ORANGE:
        return {100, 56, 9};
    case ColorMix::GREEN:
        return getRGB(Color::GREEN);
    case ColorMix::TEAL:
        return {20, 50, 33};
    case ColorMix::LIGHT_GREEN:
        return {7, 100, 13};
    case ColorMix::BLUE:
        return getRGB(Color::BLUE);
    case ColorMix::YELLOW:
        return getRGB(Color::YELLOW);
    default:
        throw std::runtime_error("Unexpected ColorMix enum value! Guess you lost the captcha^^");
    }
}

Color getRandomColor() {
    int index = (std::rand() % (COLOR_COUNT - 1)) + 1;
    return static_cast<Color>(index);
}
