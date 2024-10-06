#ifndef ansi_codes_h
#define ansi_codes_h

#define ANSI_ESCAPE "\033["

enum class AnsiStyle { RESET = 0, BOLD = 1, DIM = 2, UNDERLINED = 4, BLINK = 5, REVERSE = 7, HIDDEN = 8 };

enum class AnsiForegroundColor {
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37
};

enum class AnsiBackgroundColor {
    BLACK = 40,
    RED = 41,
    GREEN = 42,
    YELLOW = 43,
    BLUE = 44,
    MAGENTA = 45,
    CYAN = 46,
    WHITE = 47
};

#endif