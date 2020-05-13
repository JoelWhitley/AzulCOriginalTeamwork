#ifndef AZUL_TYPES
#define AZUL_TYPES

// Game sizes and counts
//size determines rows/cols of mosaic, rows of storage, and amount of tile types
#define SIZE           5
#define FACTORIES      5
#define FACTORY_SIZE   4

// Tile codes
#define FIRST_PLAYER  'F'
#define RED           'R'
#define YELLOW        'Y'
#define BLUE          'B'
#define LIGHT_BLUE    'L'
#define BLACK         'U'
#define NO_TILE       '.'

// Alias for context + code clarity
typedef char Tile;

#endif // AZUL_TYPES