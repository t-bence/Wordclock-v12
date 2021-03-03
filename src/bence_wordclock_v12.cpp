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

#include <Wire.h>
#include <EasyButton.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <DS3231.h>
#include <TimeLib.h>        // http://www.arduino.cc/playground/Code/Time
#include <Timezone.h>       // https://github.com/JChristensen/Timezone

// include the words
#include <WordclockWords.h>
#include <Constants.h>

#define LAYOUT RIGHT_TO_LEFT
#define LANGUAGE HUNGARIAN

TimeChangeRule *tcr;        //pointer to the time change rule, use to get TZ abbrev
time_t utc, local;

// init RTC
DS3231 rtc(SDA, SCL);
Time t;

// neopixel setup
#define PIN               12
#define NUM_PIXELS         110

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

int currentHour = 0;
int currentMinute = 0;
int previousMinute = 99; // init to other value

// colors
#define COLOR_BUTTON_PIN   2   // to adjust the color
byte chosenColor = 12; // the color currently stored

//Set the color globally
uint32_t colorOut = colors[chosenColor];

const int EEPROM_ADDR = 0;

// whether the screen neds to be redrawn
bool redraw = true;
#define LONG_PRESS_DURATION 1000
EasyButton button(COLOR_BUTTON_PIN);

bool characters[NUM_PIXELS] { false };


// change color and force redraw
void changeColor() {

    chosenColor++;
    if (chosenColor >= colorsDefined) { chosenColor = 0; }
    colorOut = colors[chosenColor];

    EEPROM.write(EEPROM_ADDR, chosenColor);

    redraw = true;
}

void show()
{
    for (uint8_t i = 0; i < NUM_PIXELS; i++)
    {
        if (characters[i])
        {
            pixels.setPixelColor(i, colorOut);
        }
        else
        {
            pixels.setPixelColor(i, 0, 0, 0);
        }
    }
    pixels.show();
}

// write everything with a shade color 0-255
void show(uint8_t intensity)
{
    float ratio = static_cast<float>(intensity) / 255;
    uint8_t red = colorOut >> 16;
    uint8_t green = (colorOut & (0xFF << 8)) >> 8;
    uint8_t blue = colorOut & 0xFF;

    //Serial.println();
    //Serial.print(red); Serial.print(" "); Serial.print(green);
    //Serial.print(" "); Serial.println(blue);

    uint8_t r = static_cast<uint8_t>(ratio * red);
    uint8_t g = static_cast<uint8_t>(ratio * green);
    uint8_t b = static_cast<uint8_t>(ratio * blue);

    //Serial.print(r); Serial.print(" "); Serial.print(g);
    //Serial.print(" "); Serial.println(b);

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

void write(byte data[]) {
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
    show();
}

void writeTime(int hour, int min)
{
    clear();
    # if LANGUAGE == HUNGARIAN
        // Hungarian version
        if (min < 5)       { write(MOST); write(ORA); write(VAN); }
        else if (min < 10) { write(ORA); write(MULT); write(OT_MULT); write(PERCCEL); }
        else if (min < 15) { write(ORA); write(MULT); write(TIZ_MULT); write(PERCCEL); }
        else if (min < 20) { write(MOST); write(NEGYED); write(VAN); }
        else if (min < 25) { write(NEGYED); write(MULT); write(OT_MULT); write(PERCCEL); }
        else if (min < 30) { write(OT_MULVA); write(PERC); write(MULVA); write(FEL); }
        else if (min < 35) { write(MOST); write(FEL); write(VAN); }
        else if (min < 40) { write(FEL); write(MULT); write(OT_MULT); write(PERCCEL); }
        else if (min < 45) { write(FEL); write(MULT); write(TIZ_MULT); write(PERCCEL); }
        else if (min < 50) { write(MOST); write(HAROMNEGYED); write(VAN); }
        else if (min < 55) { write(TIZ_MULVA); write(PERC); write(MULVA); write(ORA); }
        else { write(OT_MULVA); write(PERC); write(MULVA); write(ORA); }

        // show next hour eg: 5:15 -> "negyed hat"
        if (min >= 15) { hour++; }

    # else // English
        write(IT); write(IS);

        if (min < 5) { write(OCLOCK); }
        else if (min < 10) { write(FIVE_MIN); write(PAST); }
        else if (min < 15) { write(TEN_MIN); write(PAST); }
        else if (min < 20) { write(QUARTER); write(PAST); }
        else if (min < 25) { write(TWENTY); write(PAST); }
        else if (min < 30) { write(TWENTY); write(FIVE_MIN); write(PAST); }
        else if (min < 35) { write(HALF); write(PAST); }
        else if (min < 40) { write(TWENTY); write(FIVE_MIN); write(TO); }
        else if (min < 45) { write(TWENTY); write(TO); }
        else if (min < 50) { write(QUARTER); write(TO); }
        else if (min < 55) { write(TEN_MIN); write(TO); }
        else { write(FIVE_MIN); write(TO); }

        if (min >= 35) { showHour ++; }
    # endif
    
    // csak 12 orat jelzunk, a nulla is 12
    if (hour == 0) { hour = 12; }
    else if (hour > 12) { hour -= 12; }

    write(HOURS[hour - 1]);

    # if LANGUAGE == HUNGARIAN
    // in the Hungarian version, TIZEN+EGY is built from two words
    if (hour == 11) { write(HOURS[0]); }
    # endif
}

void fadeOut()
{
    for (uint8_t brightness = 255; brightness > 0; --brightness)
    {
        Serial.print("FADEOUT "); Serial.println(brightness);
        show(brightness);
        delay(5); // milliseconds
    }
}

void fadeIn()
{
    for (uint8_t brightness = 0; brightness < 255; ++brightness)
    {
        Serial.print("FADEIN "); Serial.println(brightness);
        show(brightness);
        delay(5); // milliseconds
    }
}

bool hasTimeChanged(const int currentMinute, const int previousMinute)
{
    // if the clock should be redrawn
    // later redraw should only be issued every five minutes
    return currentMinute != previousMinute;
}

void setup() {
    Serial.begin(9600); // debugging only
    Serial.println("Starting up...");
    Wire.begin();
    Serial.println("Wire has begun");

    rtc.begin();
    // Serial.println("RTC has begun");

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
    Serial.println("NeoPixel has begun");

    redraw = true; // write time immediately
}

void loop() {
    utc = (time_t) rtc.getUnixTime(rtc.getTime());
    // printTime(utc, "UTC");
    local = hunTZ.toLocal(utc, &tcr);
    // printTime(local, tcr -> abbrev);
    currentHour = hour(local);
    currentMinute = minute(local);
    redraw |= hasTimeChanged(currentMinute, previousMinute);

    if (redraw) {
        Serial.println("Redrawing...");
        fadeOut();
        writeTime(currentHour, currentMinute);
        show();
        fadeIn();

        previousMinute = currentMinute;
        redraw = false;
    }
    button.read();

}
