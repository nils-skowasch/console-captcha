#include "color_table.h"
#include <cstdlib>
#include <stdexcept>

RGB getRGB(Color color) {
    switch (color) {
    case Color::None:
        return {42, 42, 42};
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
        return getRGB(Color::None);
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

const char *getName(Color color) {
    switch (color) {
    case Color::None:
        return "None";
    case Color::RED:
        return "Red";
    case Color::GREEN:
        return "Green";
    case Color::BLUE:
        return "Blue";
    case Color::YELLOW:
        return "Yellow";
    default:
        throw std::runtime_error("Unexpected Color enum value! Guess you lost the captcha^^");
    }
}

const char *getName(ColorMix colorMix) {
    switch (colorMix) {
    case ColorMix::None:
        return getName(Color::None);
    case ColorMix::RED:
        return getName(Color::RED);
    case ColorMix::BROWN:
        return "Brown";
    case ColorMix::VIOLET:
        return "Violet";
    case ColorMix::ORANGE:
        return "Orange";
    case ColorMix::GREEN:
        return getName(Color::GREEN);
    case ColorMix::TEAL:
        return "Teal";
    case ColorMix::LIGHT_GREEN:
        return "Light Green";
    case ColorMix::BLUE:
        return getName(Color::BLUE);
    case ColorMix::YELLOW:
        return getName(Color::YELLOW);
    default:
        throw std::runtime_error("Unexpected ColorMix enum value! Guess you lost the captcha^^");
    }
}

unsigned char toChar(Color color) {
    return static_cast<unsigned char>(static_cast<int>(color) + 64); // convert Color index to characters A-D
}

Color toColor(unsigned char character) {
    int colorIndex;
    if (character >= COLOR_CHAR_START_UPPER && character <= COLOR_CHAR_END_UPPER) {
        colorIndex = static_cast<int>(character) - 64;
    } else if (character >= COLOR_CHAR_START_LOWER && character <= COLOR_CHAR_END_LOWER) {
        colorIndex = static_cast<int>(character) - 96;
    } else {
        throw std::runtime_error("Unexpected character given!");
    }
    return static_cast<Color>(colorIndex);
}

Color getRandomColor() {
    int index = (std::rand() % (COLOR_COUNT - 1)) + 1;
    return static_cast<Color>(index);
}
