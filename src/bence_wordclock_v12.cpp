/*
Wordclock source code v12

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

// should it use std::bitmask instead of the bool array?

#define LAYOUT RIGHT_TO_LEFT
#define LANGUAGE HUNGARIAN
#define BDAY_MONTH 3
#define BDAY_DAY 25
// #define DEBUG

#include <Wire.h>
#include <EasyButton.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <DS3231.h>
#include <TimeLib.h>        // http://www.arduino.cc/playground/Code/Time
#include <Timezone.h>       // https://github.com/JChristensen/Timezone
#include <DebugUtils.h>

// include the words
#include <WordclockWords.h>
#include <Constants.h>

TimeChangeRule *tcr;        // pointer to the time change rule, use to get TZ abbrev
time_t utc, local;

// init RTC
DS3231 rtc(SDA, SCL);
Time t;

// neopixel setup
const int PIN { 12 };
const int NUM_PIXELS { 110 };

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

int currentHour = 0;
int currentMinute = 0;
int previousMinute = 99; // init to other value

// colors
const int COLOR_BUTTON_PIN { 2 };  // to adjust the color
byte chosenColor = 12; // the color currently stored
uint32_t colorOut = colors[chosenColor];

const int EEPROM_ADDR = 13;

// whether the screen neds to be redrawn
bool redraw = true;
const int LONG_PRESS_DURATION { 1000 };
EasyButton button(COLOR_BUTTON_PIN);

// state of leds for each letter
bool characters[NUM_PIXELS] { false };

// no fade at start & color change
bool fade { false };


// change color and force redraw
void changeColor() {

    chosenColor++;
    if (chosenColor >= colorsDefined) { chosenColor = 0; }
    colorOut = colors[chosenColor];

    EEPROM.write(EEPROM_ADDR, chosenColor);

    fade = false;
    redraw = true;
}


// show current buffer with intensity (0-255)
void showBuffer(uint8_t intensity)
{
    float ratio = static_cast<float>(intensity) / 255;
    uint8_t red = colorOut >> 16;
    uint8_t green = (colorOut & (0xFF << 8)) >> 8;
    uint8_t blue = colorOut & 0xFF;

    DEBUG_PRINTLN();
    DEBUG_PRINT(red); DEBUG_PRINT(" "); DEBUG_PRINT(green);
    DEBUG_PRINT(" "); DEBUG_PRINTLN(blue);

    uint8_t r = static_cast<uint8_t>(ratio * red);
    uint8_t g = static_cast<uint8_t>(ratio * green);
    uint8_t b = static_cast<uint8_t>(ratio * blue);

    DEBUG_PRINT(r); DEBUG_PRINT(" "); DEBUG_PRINT(g);
    DEBUG_PRINT(" "); DEBUG_PRINTLN(b);

    for (uint8_t i = 0; i < NUM_PIXELS; i++)
    {
        if (characters[i])
        {
            pixels.setPixelColor(i, r, g, b);
        }
        else
        {
            pixels.setPixelColor(i, 0, 0, 0);
        }
    }
    pixels.show();
}

// show current buffer with full intensity
void showBuffer()
{
    showBuffer(255);
}

void toBuffer(byte data[]) {
    byte start = data[0];
    byte len = data[1];

    for (int i = 0; i <= len; i++)
    {
        characters[start + i] = true;
    }
}

void clear()
{
    for (uint8_t i = 0; i < NUM_PIXELS; i++)
    {
        characters[i] = false;
    }
    showBuffer();
}

void writeTime(int hour, int min)
{
    clear();
    # if LANGUAGE == HUNGARIAN
        // Hungarian version
        if (min < 5)       { toBuffer(MOST); toBuffer(ORA); toBuffer(VAN); }
        else if (min < 10) { toBuffer(ORA); toBuffer(MULT); toBuffer(OT_MULT); toBuffer(PERCCEL); }
        else if (min < 15) { toBuffer(ORA); toBuffer(MULT); toBuffer(TIZ_MULT); toBuffer(PERCCEL); }
        else if (min < 20) { toBuffer(MOST); toBuffer(NEGYED); toBuffer(VAN); }
        else if (min < 25) { toBuffer(NEGYED); toBuffer(MULT); toBuffer(OT_MULT); toBuffer(PERCCEL); }
        else if (min < 30) { toBuffer(OT_MULVA); toBuffer(PERC); toBuffer(MULVA); toBuffer(FEL); }
        else if (min < 35) { toBuffer(MOST); toBuffer(FEL); toBuffer(VAN); }
        else if (min < 40) { toBuffer(FEL); toBuffer(MULT); toBuffer(OT_MULT); toBuffer(PERCCEL); }
        else if (min < 45) { toBuffer(FEL); toBuffer(MULT); toBuffer(TIZ_MULT); toBuffer(PERCCEL); }
        else if (min < 50) { toBuffer(MOST); toBuffer(HAROMNEGYED); toBuffer(VAN); }
        else if (min < 55) { toBuffer(TIZ_MULVA); toBuffer(PERC); toBuffer(MULVA); toBuffer(ORA); }
        else { toBuffer(OT_MULVA); toBuffer(PERC); toBuffer(MULVA); toBuffer(ORA); }

        // show next hour eg: 5:15 -> "negyed hat"
        if (min >= 15) { hour++; }

    # else // English
        toBuffer(IT); toBuffer(IS);

        if (min < 5) { toBuffer(OCLOCK); }
        else if (min < 10) { toBuffer(FIVE_MIN); toBuffer(PAST); }
        else if (min < 15) { toBuffer(TEN_MIN); toBuffer(PAST); }
        else if (min < 20) { toBuffer(QUARTER); toBuffer(PAST); }
        else if (min < 25) { toBuffer(TWENTY); toBuffer(PAST); }
        else if (min < 30) { toBuffer(TWENTY); toBuffer(FIVE_MIN); toBuffer(PAST); }
        else if (min < 35) { toBuffer(HALF); toBuffer(PAST); }
        else if (min < 40) { toBuffer(TWENTY); toBuffer(FIVE_MIN); toBuffer(TO); }
        else if (min < 45) { toBuffer(TWENTY); toBuffer(TO); }
        else if (min < 50) { toBuffer(QUARTER); toBuffer(TO); }
        else if (min < 55) { toBuffer(TEN_MIN); toBuffer(TO); }
        else { toBuffer(FIVE_MIN); toBuffer(TO); }

        if (min >= 35) { showHour ++; }
    # endif
    
    // csak 12 orat jelzunk, a nulla is 12
    if (hour == 0) { hour = 12; }
    else if (hour > 12) { hour -= 12; }

    toBuffer(HOURS[hour - 1]);

    # if LANGUAGE == HUNGARIAN
    // in the Hungarian version, TIZEN+EGY is built from two words
    if (hour == 11) { toBuffer(HOURS[0]); }
    # endif
}

void fadeOut()
{
    for (uint8_t brightness = 255; brightness > 0; --brightness)
    {
        DEBUG_PRINT("FADEOUT "); DEBUG_PRINTLN(brightness);
        showBuffer(brightness);
        delay(15); // milliseconds
    }
}

void fadeIn()
{
    for (uint8_t brightness = 0; brightness < 255; ++brightness)
    {
        DEBUG_PRINT("FADEIN "); DEBUG_PRINTLN(brightness);
        showBuffer(brightness);
        delay(15); // milliseconds
    }
}

void greetBirthday()
{
    clear();
    for (size_t i = 0; i < NUM_BIRTHDAY_LETTERS; i++)
    {
        byte data[] {BIRTHDAY_LETTERS[i], 0};
        toBuffer(data);
    }
    showBuffer();
}


bool hasTimeChanged(const int currentMinute, const int previousMinute)
{
    // if the clock should be redrawn
    // should only be issued every five minutes
    return (currentMinute / 5) != (previousMinute / 5);
}

void setup() {
    Serial.begin(9600); // debugging only
    DEBUG_PRINTLN("Starting up...");
    Wire.begin();
    DEBUG_PRINTLN("Wire has begun");

    rtc.begin();
    DEBUG_PRINTLN("RTC has begun");

    // button
    button.begin();
    button.onPressedFor(LONG_PRESS_DURATION, changeColor);

    // read from EEPROM.
    chosenColor = EEPROM.read(EEPROM_ADDR);
    if (chosenColor >= colorsDefined) {
        chosenColor = 0;
    }
    colorOut = colors[chosenColor];

    pixels.begin();
    pixels.setBrightness(255); // 0-255
    pixels.clear();

    pixels.show();  // reset to no colors
    DEBUG_PRINTLN("NeoPixel has begun");

    redraw = true; // write time immediately
}

void loop() {
    utc = (time_t) rtc.getUnixTime(rtc.getTime());
    local = hunTZ.toLocal(utc, &tcr);
    currentHour = hour(local);
    currentMinute = minute(local);
    redraw |= hasTimeChanged(currentMinute, previousMinute);

    if (redraw) {
        if ((month(local) == BDAY_MONTH) && (day(local) == BDAY_DAY)) {
            greetBirthday();
        }
        else
        {
            DEBUG_PRINTLN("Redrawing...");
            fade ? fadeOut() : clear();
            writeTime(currentHour, currentMinute);
            fade ? fadeIn() : showBuffer(255);
            fade = true;
        }

        previousMinute = currentMinute;
        redraw = false;
    }
    button.read();

}
