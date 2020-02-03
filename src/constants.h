// függvények
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#ifndef WORDCLOCKWORDS_H
#define WORDCLOCKWORDS_H

// language and text direction
#define HUNGARIAN 0
#define ENGLISH 1

#define LEFT_TO_RIGHT 0
#define RIGHT_TO_LEFT 1


// colors
const uint32_t colors[] =
{
    Adafruit_NeoPixel::Color(127, 127, 0), // sarga
    Adafruit_NeoPixel::Color(250, 250, 0),
    Adafruit_NeoPixel::Color(127, 0, 127),
    Adafruit_NeoPixel::Color(250, 0, 250), // pink
    Adafruit_NeoPixel::Color(0, 127, 127), // cian
    Adafruit_NeoPixel::Color(0, 250, 250), // cian
    Adafruit_NeoPixel::Color(127, 0, 0), // piros
    Adafruit_NeoPixel::Color(255, 0, 0), // piros
    Adafruit_NeoPixel::Color(0, 127, 0),
    Adafruit_NeoPixel::Color(0, 250, 0), // zold
    Adafruit_NeoPixel::Color(0, 0, 127),
    Adafruit_NeoPixel::Color(0, 0, 255), // kek
    Adafruit_NeoPixel::Color(127, 127, 127), // halvany feher
    Adafruit_NeoPixel::Color(250, 250, 250), // eros feher
    Adafruit_NeoPixel::Color(250, 100, 0) // narancssarga
};
static byte colorsDefined = 15;

# if LANGUAGE == HUNGARIAN
# if TEXT_DIRECTION == LEFT_TO_RIGHT
byte HOURS[12][2] = {
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
  {61, 4}, // TIZEN-, ehhez mutatni kell az EGY-et is!
  {56, 9}, // 12
};

// ha szembol nezve bal oldalt kezdodik a LED-sor...
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
byte HAROMNEGYED[]= {88, 10};


# else
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
byte EGY[] = {47, 2};
byte KETTO[] = {60, 4};
byte HAROM[] = {35, 4};
byte NEGY[] = {26, 3};
byte OT[] = {40, 1};
byte HAT[] = {30, 2};
byte HET[] = {44, 2};
byte NYOLC[] = {50, 4};
byte KILENC[] = {67, 5};
byte TIZ[] = {41, 2};
byte TIZENEGY1[] = {55, 4};
byte TIZENEGY2[] = {47, 2};
byte TIZENKETTO[] = {55, 9};
# endif

# elif LANGUAGE == ENGLISH
# if TEXT_DIRECTION LEFT_TO_RIGHT
byte HOURS[12][2] = {
  {52, 2},
  {49, 2},
  {44, 4},
  {33, 3},
  {37, 3},
  {41, 2},
  {60, 4},
  {11, 4},
  {29, 3},
  {8, 2},
  {16, 5},
  {22, 5}
};

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

# else // right-to-left
byte HOURS {};
# endif
#endif

#endif
