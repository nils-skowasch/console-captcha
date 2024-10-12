
#ifndef color_table_h
#define color_table_h

#define COLOR_COUNT 4

struct RGB {
    int red;
    int green;
    int blue;
};

enum class Color { None, RED, GREEN, BLUE, YELLOW };

enum class ColorMix {
    None,
    RED,         // RED + RED
    BROWN,       // RED + GREEN
    VIOLET,      // RED + BLUE
    ORANGE,      // RED + YELLOW
    GREEN,       // GREEN + GREEN, BLUE + YELLOW
    TEAL,        // GREEN + BLUE
    LIGHT_GREEN, // GREEN + YELLOW
    BLUE,        // BLUE + BLUE
    YELLOW       // YELLOW + YELLOW
};

RGB getRGB(Color color);
RGB getRGB(ColorMix colorMix);

Color getRandomColor();

#endif