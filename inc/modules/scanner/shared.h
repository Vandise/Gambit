#ifndef __GAMBIT_SCANNER_SHAREDH
#define __GAMBIT_SCANNER_SHAREDH 1

// ============================
//           Macros
// ============================

#define EOF_CHAR '\x7f'
#define NEWLINE_CHAR '\n'
#define COMMENT_CHAR '#'

// ============================
//       Structs & Types
// ============================

typedef enum {
    LETTER,
    UPPERCASE_LETTER,
    DIGIT,
    QUOTE,
    UNDERSCORE,
    SPECIAL,
    NEWLINE,
    EOF_CODE
} CHAR_CODE;

#endif