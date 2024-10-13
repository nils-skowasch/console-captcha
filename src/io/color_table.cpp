#include "color_table.h"
#include <cstdlib>
#include <stdexcept>

RGB getRGB(Color color) {
    switch (color) {
    case Color::None:
        return {200, 200, 200};
    case Color::RED:
        return {255, 0, 0};
    case Color::GREEN:
        return {0, 255, 0};
    case Color::BLUE:
        return {0, 0, 255};
    case Color::YELLOW:
        return {238, 225, 12};
    default:
        throw std::runtime_error("Unexpected Color enum value! Guess you lost the captcha^^");
    }
}

RGB getRGB(ColorMix colorMix) {
    switch (colorMix) {
    case ColorMix::None:
        return getRGB(Color::None);
    case ColorMix::RED:
        return {128, 0, 0};
    case ColorMix::BROWN:
        return {61, 40, 0};
    case ColorMix::VIOLET:
        return {62, 0, 64};
    case ColorMix::ORANGE:
        return {100, 56, 9};
    case ColorMix::GREEN:
        return {0, 128, 0};
    case ColorMix::TEAL:
        return {20, 50, 33};
    case ColorMix::LIGHT_GREEN:
        return {7, 100, 13};
    case ColorMix::BLUE:
        return {0, 0, 128};
    case ColorMix::YELLOW:
        return {135, 128, 7};
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

Color getRandomColor() {
    int index = (std::rand() % COLOR_COUNT) + 1;
    return static_cast<Color>(index);
}

ColorMix merge(Color color0, Color color1) {
    return merge(color0, color1, true);
}

ColorMix merge(Color color0, Color color1, bool forwards) {
    switch (color0) {
    case Color::None:
        break;
    case Color::RED:
        switch (color1) {
        case Color::None:
            break;
        case Color::RED:
            return ColorMix::RED;
        case Color::GREEN:
            return ColorMix::BROWN;
        case Color::BLUE:
            return ColorMix::VIOLET;
        case Color::YELLOW:
            return ColorMix::ORANGE;
        }
        break;
    case Color::GREEN:
        switch (color1) {
        case Color::None:
        case Color::RED:
            break;
        case Color::GREEN:
            return ColorMix::GREEN;
        case Color::BLUE:
            return ColorMix::TEAL;
        case Color::YELLOW:
            return ColorMix::LIGHT_GREEN;
        }
        break;
    case Color::BLUE:
        switch (color1) {
        case Color::None:
        case Color::RED:
        case Color::GREEN:
            break;
        case Color::YELLOW:
            return ColorMix::LIGHT_GREEN;
        case Color::BLUE:
            return ColorMix::BLUE;
        }
        break;
    case Color::YELLOW:
        switch (color1) {
        case Color::None:
        case Color::RED:
        case Color::GREEN:
            break;
        case Color::BLUE:
            return ColorMix::GREEN;
        case Color::YELLOW:
            return ColorMix::YELLOW;
        }
        break;
    }

    // nothing found yet
    if (forwards) {
        return merge(color1, color0, false); // try to find a match merging backwards
    } else {
        return ColorMix::None; // no match found
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
