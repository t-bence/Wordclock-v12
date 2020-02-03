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

// using Rinky-Dinky electronics code by Hanning Karlsen

#include <Arduino.h>
#include <Wire.h>
#include <DS3231.h>
#include <TimeLib.h>        // http://www.arduino.cc/playground/Code/Time
#include <Timezone.h>       // https://github.com/JChristensen/Timezone
#include "Adafruit_NeoPixel.h"
#include "constants.h"


// egyéni beállítások
#define DEBUG 1 // legyen-e debug
#define LANGUAGE HUNGARIAN
#define TEXT_DIRECTION LEFT_TO_RIGHT

const byte LED_PIN {12};
const byte COLOR_BUTTON_PIN {2};
const bool ANIMATE {false};

// predefine redraw function
void colorButtonPressed();

const byte NUM_PIXELS {110};
const byte BRIGHTNESS {255};

class Wordclock {
    private: // default anyways
        bool animate_;
        int hour_{ -1 };
        int min_{ -1 };
        DS3231 rtc_{ SDA, SCL };

        volatile unsigned long colorLastPressed_ {0};
        const unsigned long buttonPeriod_ {400};
        volatile byte colorIndex_ {0};

        Adafruit_NeoPixel pixels_;

        // set up rules for changing time
        //Daylight time = UTC + 2 hours
        TimeChangeRule myDST_{ "CEST", Last, Sun, Mar, 2, 120 };
        //Standard time = UTC + 1 hours
        TimeChangeRule mySTD_{ "CET", Last, Sun, Oct, 3, 60 };
        Timezone hunTZ_{ myDST_, mySTD_ };

        void show(byte data[])
        {
            for (int i = 0; i <= data[1]; i++) {
                pixels_.setPixelColor(data[0] + i, colors[colorIndex_]);
            }
        }

    public:
        Wordclock(byte ledPin, byte colorButtonPin, bool animate)
        {
            animate_ = animate;
            pixels_ = Adafruit_NeoPixel(NUM_PIXELS, ledPin,
                                        NEO_GRB + NEO_KHZ800);
            pixels_.begin();
            pixels_.setBrightness(BRIGHTNESS);
            pixels_.clear();

            // set up color change interrupt
            pinMode(colorButtonPin, INPUT_PULLUP);
            attachInterrupt(digitalPinToInterrupt(colorButtonPin),
                colorButtonPressed, RISING);

            rtc_.begin();
            update();

        }

        // update time
        bool update()
        {
            time_t utc = (time_t) rtc_.getUnixTime(rtc_.getTime());
            time_t local = hunTZ_.toLocal(utc);

            bool redraw {false};

            int currHour = hour(local);
            int currMin = minute(local);
            // check if time has changed by five minutes
            // if true, set changed_ to true
            if ((currHour != hour_) || (abs(currMin - min_) >= 5)) // if time has changed by five minutes
            {
                redraw = true;
                if (animate_)
                {
                // do animation
                }
                print();
            }
            else {
                redraw = false;
            }
            hour_ = currHour;
            min_ = currHour;

            return redraw;
        }

        void changeColor()
        {
            unsigned long now = millis();
            // second case can happen when millis overflows
            if ((now - colorLastPressed_ > buttonPeriod_) || (now < colorLastPressed_)) {
                if (colorIndex_ >= colorsDefined)
                {
                    colorIndex_ = 0;
                }
                else {
                    colorIndex_++;
                }
                colorLastPressed_ = now;
                print();
            }
        }

        void print()
        {
            // actually draw on the screen and things
            // this will be called on color change, too
            // use prev time stored in the object
            // also color
            // hour_, min_, colorIndex_

            pixels_.clear();

            int showHour = hour_;
            # if LANGUAGE == HUNGARIAN

                // Hungarian version
                if (min_ < 5)       { show(MOST); show(ORA); show(VAN); }
                else if (min_ < 10) { show(ORA); show(MULT); show(OT_MULT); show(PERCCEL); }
                else if (min_ < 15) { show(ORA); show(MULT); show(TIZ_MULT); show(PERCCEL); }
                else if (min_ < 20) { show(MOST); show(NEGYED); show(VAN); }
                else if (min_ < 25) { show(NEGYED); show(MULT); show(OT_MULT); show(PERCCEL); }
                else if (min_ < 30) { show(OT_MULVA); show(PERC); show(MULVA); show(FEL); }
                else if (min_ < 35) { show(MOST); show(FEL); show(VAN); }
                else if (min_ < 40) { show(FEL); show(MULT); show(OT_MULT); show(PERCCEL); }
                else if (min_ < 45) { show(FEL); show(MULT); show(TIZ_MULT); show(PERCCEL); }
                else if (min_ < 50) { show(MOST); show(HAROMNEGYED); show(VAN); }
                else if (min_ < 55) { show(TIZ_MULVA); show(PERC); show(MULVA); show(ORA); }
                else { show(OT_MULVA); show(PERC); show(MULVA); show(ORA); }

                // show next hour eg: 5:15 -> "negyed hat"
                if (min_ >= 15) { showHour++; }

            # else // English
                show(IT); show(IS);

                if (min_ < 5) { show(OCLOCK); }
                else if (min_ < 10) { show(FIVE_MIN); show(PAST); }
                else if (min_ < 15) { show(TEN_MIN); show(PAST); }
                else if (min_ < 20) { show(QUARTER); show(PAST); }
                else if (min_ < 25) { show(TWENTY); show(PAST); }
                else if (min_ < 30) { show(TWENTY); show(FIVE_MIN); show(PAST); }
                else if (min_ < 35) { show(HALF); show(PAST); }
                else if (min_ < 40) { show(TWENTY); show(FIVE_MIN); show(TO); }
                else if (min_ < 45) { show(TWENTY); show(TO); }
                else if (min_ < 50) { show(QUARTER); show(TO); }
                else if (min_ < 55) { show(TEN_MIN); show(TO); }
                else { show(FIVE_MIN); show(TO); }

                if (min_ >= 35) { showHour ++; }
            # endif
            
            // csak 12 orat jelzunk, a nulla is 12
            if (showHour == 0) { showHour = 12; }
            else if (showHour > 12) { showHour -= 12; }

            show(HOURS[hour_ - 1]);

            # if LANGUAGE == HUNGARIAN
            // in the Hungarian version, TIZEN+EGY is built from two words
            if (hour_ == 11) { show(HOURS[0]); }
            # endif

            pixels_.show();
        }

};

Wordclock wordclock = Wordclock(LED_PIN, COLOR_BUTTON_PIN, ANIMATE);

void colorButtonPressed() { wordclock.changeColor(); }

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting up");

    Wire.begin();
    Serial.println("Wire has begun");
    Serial.println("RTC has begun");

}

void loop()
{
    if (wordclock.update())
    {
        wordclock.print();
    }
    
}