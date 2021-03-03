// függvények
#include <Arduino.h>

#define LEFT_TO_RIGHT 0
#define RIGHT_TO_LEFT 1

#define HUNGARIAN 0
#define ENGLISH 1


// ha szembol nezve bal oldalt kezdodik a LED-sor...
#if LAYOUT == LEFT_TO_RIGHT
byte MOST[] = {105, 3};
byte ORA[]  = {30, 2};
byte VAN[]  = {14, 2};
byte MULT[] = {18, 3};
byte OT_MULT[] = {11, 1};  // a lent szereplo ot, tehat az ot perccel mult
byte TIZ_MULT[] = {0, 2}; // a lent szereplo tiz, a tiz perccel mult
byte OT_MULVA[] = {102, 1}; // a fent szereplo ot, tehat az ot perc mulva
byte TIZ_MULVA[] = {100, 2}; // a fent szereplo tiz; tiz perc mulva
byte PERC[] = {84, 3};
byte PERCCEL[] = {4, 6};
byte NEGYED[]= {93, 5};
byte MULVA[] = {78, 4};
byte FEL[] = {66, 2};
byte HAROMNEGYED[] = {88, 10};

// orak
uint8_t HOURS[12][2] {
  {49, 2},
  {56, 4},
  {37, 4},
  {25, 3},
  {35, 1},
  {22, 2},
  {52, 2},
  {44, 4},
  {70, 5},
  {33, 2},
  {61, 4},
  {56, 9}
};
#endif


// ha szembol nezve jobb oldalon kezdodik a LED-sor
#if LAYOUT == RIGHT_TO_LEFT
byte MOST[] = {100, 3};
byte ORA[]  = {22, 2};
byte VAN[]  = {16, 2};
byte MULT[] = {11, 3};
byte OT_MULT[] = {20, 1};  // a lent szereplo ot, tehat az ot perccel mult
byte TIZ_MULT[] = {8, 2}; // a lent szereplo tiz, a tiz perccel mult
byte OT_MULVA[] = {105, 1}; // a fent szereplo ot, tehat az ot perc mulva
byte TIZ_MULVA[] = {106, 2}; // a fent szereplo tiz; tiz perc mulva
byte PERC[] = {77, 3};
byte PERCCEL[] = {0, 6};
byte NEGYED[]= {88, 5};
byte MULVA[] = {82, 4};
byte FEL[] = {74, 2};
byte HAROMNEGYED[]= {88, 10};
// orak
uint8_t HOURS[12][2] {
  {47, 2},
  {60, 4},
  {35, 4},
  {26, 3},
  {40, 1},
  {30, 2},
  {44, 2},
  {50, 4},
  {67, 5},
  {41, 2},
  {55, 4},
  {55, 9}
};
#endif


// angolul - ha szembol nezve jobbrol indul a led szalag
#if LAYOUT == RIGHT_TO_LEFT
// szavak, percek
byte IT[] = {99, 1};
byte IS[]  = {102, 1};
byte QUARTER[] = {91, 6};
byte TWENTY[] = {77, 5};
byte FIVE_MIN[] = {84, 3};
byte HALF[] = {73, 3};
byte TEN_MIN[] = {69, 2};
byte TO[] = {66, 1};
byte PAST[] = {55, 3};
byte OCLOCK[] = {0, 5};


// orak
byte ONE[] = {52, 2};
byte TWO[] = {49, 2};
byte THREE[] = {44, 4};
byte FOUR[] = {33, 3};
byte FIVE[] = {37, 3};
byte SIX[] = {41, 2};
byte SEVEN[] = {60, 4};
byte EIGHT[] = {11, 4};
byte NINE[] = {29, 3};
byte TEN[] = {8, 2};
byte ELEVEN[] = {16, 5};
byte TWELVE[] = {22, 5};
#endif


// ANGOLUL, ha szembol nezve balrol indul a led szalag
#if LAYOUT == LEFT_TO_RIGHT
byte IT[] = {108, 1};
byte IS[] = {105, 1};
byte QUARTER[] = {89, 6};
byte TWENTY[] = {82, 5};
byte FIVE_MIN[] = {77, 3};
byte HALF[] = {66, 3};
byte TEN_MIN[] = {71, 2};
byte TO[] = {75, 1};
byte PAST[] = {62, 3};
byte OCLOCK[] = {5, 5};

byte ONE[] = {44, 2};
byte TWO[] = {47, 2};
byte THREE[] = {50, 4};
byte FOUR[] = {40, 3};
byte FIVE[] = {36, 3};
byte SIX[] = {33, 2};
byte SEVEN[] = {56, 4};
byte EIGHT[] = {17, 4};
byte NINE[] = {22, 3};
byte TEN[] = {0, 2};
byte ELEVEN[] = {11, 5};
byte TWELVE[] = {27, 5};
#endif
