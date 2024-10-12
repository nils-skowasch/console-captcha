
#ifndef color_table_h
#define color_table_h

#define COLOR_COUNT 4
#define COLOR_MIX_COUNT 9
#define COLOR_CHAR_START_UPPER 'A'
#define COLOR_CHAR_START_LOWER 'a'
#define COLOR_CHAR_END_UPPER 'D'
#define COLOR_CHAR_END_LOWER 'd'

struct RGB {
    int red;
    int green;
    int blue;

  public:
    bool equals(RGB other) {
        return red == other.red && green == other.green && blue == other.blue;
    }
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

const char *getName(Color color);
const char *getName(ColorMix colorMix);

Color getRandomColor();

ColorMix merge(Color color0, Color color1);
ColorMix merge(Color color0, Color color1, bool forward);

unsigned char toChar(Color color);
Color toColor(unsigned char character);

#endif
