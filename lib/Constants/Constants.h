#include <Adafruit_NeoPixel.h>
#include <Timezone.h>

const uint32_t colors[] = {
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
static uint8_t colorsDefined = 15;


// timezone change rules
// for Budapest
TimeChangeRule hunDST = {"CEST", Last, Sun, Mar, 2, 120};    //Daylight time = UTC + 2 hours
TimeChangeRule hunSTD = {"CET", Last, Sun, Oct, 3, 60};     //Standard time = UTC + 1 hours
Timezone hunTZ(hunDST, hunSTD);
// for London
TimeChangeRule engDST = {"BST", Last, Sun, Mar, 2, 60};    //Daylight time = UTC + 1 hours
TimeChangeRule engSTD = {"GMT", Last, Sun, Oct, 3, 0};     //Standard time = UTC
Timezone engTZ(engDST, engSTD);
