#ifndef AZUL_RULES
#define AZUL_RULES

// Alias for context + code clarity
typedef char Tile;

// Game sizes and counts
//size determines rows/cols of mosaic, rows of storage, and amount of tile types
#define SIZE           5
#define FACTORIES      5
#define FACTORY_SIZE   4
#define FLOOR_SIZE     8

// Tile codes
#define FIRST_PLAYER  'F'
#define RED           'R'
#define YELLOW        'Y'
#define BLUE          'B'
#define LIGHT_BLUE    'L'
#define BLACK         'U'
#define NO_TILE       '.'

// Fixed arrays
static const Tile tileSelection[SIZE] = {RED,YELLOW,BLUE,LIGHT_BLUE,BLACK};
static const Tile topRowOrder[SIZE]   = {BLUE,YELLOW,RED,BLACK,LIGHT_BLUE};
static const int demerits[FLOOR_SIZE] = {0, -1, -2, -4, -6, -8, -11, -14};

// User input outcomes
enum outcome { 
    OUTCOME_TURNSUCCESS,
    OUTCOME_TURNFAIL,
    OUTCOME_SAVE,
    OUTCOME_EXIT,
    OUTCOME_INVALID,
};

#endif // AZUL_RULES