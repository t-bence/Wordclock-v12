/*
Wordclock source code v12
    Copyright (C) 2018 Bence Tóth

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

// using Rinky-Dinky electronics code by Hanning Karlsen

#include <Arduino.h>
#include <Wire.h>
#include <DS3231.h>
#include <TimeLib.h>        //http://www.arduino.cc/playground/Code/Time
#include <Timezone.h>    //https://github.com/JChristensen/Timezone

#include <WordclockDisplay.h>

#define DEBUG 1 // legyen-e debug

// neopixel data pin setup
#define PIN               12
#define LANGUAGE ENGLISH
#define colourButtonPin   2   // to adjust the colour

// auto redraw period
const unsigned long REDRAW_PERIOD = 10000;
// colour button debounce time
const unsigned long COLOUR_PRESSED_PERIOD = 400;

// set up rules for changing time
//Daylight time = UTC + 2 hours
TimeChangeRule myDST = {"CEST", Last, Sun, Mar, 2, 120};
//Standard time = UTC + 1 hours
TimeChangeRule mySTD = {"CET", Last, Sun, Oct, 3, 60};
Timezone hunTZ(myDST, mySTD);

TimeChangeRule *tcr;   //pointer to the time change rule, use to get TZ abbrev
time_t utc, local;

// init RTC
DS3231 rtc(SDA, SCL);

byte zero = 0x00; //workaround for issue #527

int intHour = 0;
int intMinute = 0;

#define NUMPIXELS 110

// to debounce the buttons
volatile unsigned long colourTimer = 0;
unsigned long redrawTimer = 0;

WordclockDisplay display = WordclockDisplay(PIN, LANGUAGE, NUMPIXELS);

// =============================== FUNCTIONS BEGIN HERE ========================

void colourButtonPressed() {
    // check when was the last press for debouncing
    if (millis() - colourTimer > COLOUR_PRESSED_PERIOD) {
        Serial.println("+++ Colour Button Pressed +++");
        //display.setNextColour();
        colourTimer = millis();
        #ifdef DEBUG
            Serial.println("Colour button pressed");
        #endif
    }
}


// ========= processing, displaying =============

void writeTime() {
    utc = (time_t) rtc.getUnixTime(rtc.getTime());
    // printTime(utc, "UTC");
    local = hunTZ.toLocal(utc, &tcr);
    // printTime(local, tcr -> abbrev);

    intHour = hour(local);
    intMinute = minute(local);

    Serial.print(intHour);
    Serial.println(intMinute);

    // display.setTime(intHour, intMinute);

    #ifdef DEBUG
        //display.debugPrintTime(utc, "UTC");
        //display.debugPrintTime(local, tcr -> abbrev);
    #endif

}

// =========================================================
void setup() {
    //#ifdef DEBUG
        Serial.begin(115200); // debugging only
        Serial.println("Starting up in DEBUG mode...");
    //#endif

    Wire.begin();
    Serial.println("Wire has begun");
    rtc.begin();
    Serial.println("RTC has begun");

    // pullup resistor, no need for extra resistor in the circuit
    pinMode(colourButtonPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(colourButtonPin), colourButtonPressed, RISING); // attach interrupt

    // writeTime();

}

void loop() {

    if (millis() - redrawTimer > REDRAW_PERIOD) {
        Serial.println("Redrawing...");
        writeTime();
        redrawTimer = millis();
    }

    Serial.println("working...");
    delay(1000);
} // end of loop()
