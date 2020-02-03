#include <WordclockDisplay.h>
#include <WordclockWords.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <TimeLib.h>

int currentHour;
int currentMinute;

byte language;
byte num_pixels;
Adafruit_NeoPixel pixels;

volatile byte chosenColour;
volatile uint32_t currentColour;
const int EEPROM_ADDR = 0;

const uint32_t colours[] = {pixels.Color(127, 127, 0), // sarga
                pixels.Color(250, 250, 0),
                pixels.Color(127, 0, 127),
                pixels.Color(250, 0, 250), // pink
                pixels.Color(0, 127, 127), // cian
                pixels.Color(0, 250, 250), // cian
                pixels.Color(127, 0, 0), // piros
                pixels.Color(255, 0, 0), // piros
                pixels.Color(0, 127, 0),
                pixels.Color(0, 250, 0), // zold
                pixels.Color(0, 0, 127),
                pixels.Color(0, 0, 255), // kek
                pixels.Color(127, 127, 127), // halvany feher
                pixels.Color(250, 250, 250), // eros feher
                pixels.Color(250, 100, 0) // narancssarga
};
static byte coloursDefined = 15;

WordclockDisplay::WordclockDisplay(byte data_pin, byte lang, byte numpixels) {
  language = lang;
  num_pixels = numpixels;

  pixels = Adafruit_NeoPixel(num_pixels, data_pin, NEO_GRB + NEO_KHZ800);

  // read from EEPROM
  chosenColour = EEPROM.read(EEPROM_ADDR);
  if (chosenColour >= coloursDefined) { // if an error has occured
    chosenColour = 0;
  }
  currentColour = colours[chosenColour];

  pixels.begin();
  pixels.setBrightness(255); // 0-255
  clearAll();

  pixels.show();  // reset to no colors

  currentHour = 0;
  currentMinute = 0;

}

void WordclockDisplay::reDraw() {
  // print new time
  if (language == HUNGARIAN)
    showTimeHun(currentHour, currentMinute);
  else
    showTimeEng(currentHour, currentMinute);
}

void WordclockDisplay::setTime(int h, int m) {
  // if there was a change, register new values and redraw,
  // otherwise do nothing , maybe this helps LED longevity

  if ((h != currentHour) or (m != currentMinute)) {
    currentHour = h;
    currentMinute = m;
    reDraw();
  }
}

void WordclockDisplay::clearAll() {
    for (int i = 0; i < num_pixels; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, 0); // switch all off
    }
}

void WordclockDisplay::setNextColour() {
  chosenColour++;
  if (chosenColour > (coloursDefined-1)) { chosenColour = 0; }
  currentColour = colours[chosenColour];

  EEPROM.write(EEPROM_ADDR, chosenColour);
  reDraw();
}

void WordclockDisplay::show(byte data[]) {
    byte start = data[0];
    byte len = data[1];

    for (int i = 0; i <= len; i++) {
        pixels.setPixelColor(start + i, currentColour);
    }
}

// ez a fv allitja be az idot...
void WordclockDisplay::showTimeHun(int hour, int min) {

    clearAll();

    bool showNext = false;

    // percek beallitasa
    // a percek / negyed orak
    if (min < 5) {
        show(MOST);
        show(ORA);
        show(VAN); // most, ora, van
    }
    else if (min < 10) {
        show(ORA);
        show(MULT);
        show(OT_MULT);
        show(PERCCEL); // ora, mult, ot, perccel
    }
    else if (min < 15) {
        show(ORA);
        show(MULT);
        show(TIZ_MULT);
        show(PERCCEL); // ora, mult, ot, perccel
    }
    else if (min < 20) {
        show(MOST);
        show(NEGYED);
        show(VAN); // most negyed van
        showNext = true;
    }
    else if (min < 25) {
        show(NEGYED);
        show(MULT);
        show(OT_MULT);
        show(PERCCEL);
        showNext = true;
    }
    else if (min < 30) {
        show(OT_MULVA);
        show(PERC);
        show(MULVA);
        show(FEL);
        showNext = true;
    }
    else if (min < 35) {
        show(MOST);
        show(FEL);
        show(VAN);
        showNext = true;
    }
    else if (min < 40) {
        show(FEL);
        show(MULT);
        show(OT_MULT);
        show(PERCCEL);
        showNext = true;
    }
    else if (min < 45) {
        show(FEL);
        show(MULT);
        show(TIZ_MULT);
        show(PERCCEL);
        showNext = true;
    }
    else if (min < 50) {
        show(MOST);
        show(HAROMNEGYED);
        show(VAN);
        showNext = true;
    }
    else if (min < 55) {
        show(TIZ_MULVA);
        show(PERC);
        show(MULVA);
        show(ORA);
        showNext = true;
    }
    else {
        show(OT_MULVA);
        show(PERC);
        show(MULVA);
        show(ORA);
        showNext = true;
    }

    if (showNext) {
        hour = hour + 1;
    }

    // csak 12 orat jelzunk, a nulla is 12
    if (hour == 0) { hour = 12; }
    else if (hour > 12) { hour -= 12; }

    show(ORAK[hour - 1]);
    if (hour == 11) {
      show(ORAK[0]);  // a TIZEN+EGY-et két részből kell összerakni
    }

    pixels.show();
}

// ez a fv allitja be az idot... angolul
void WordclockDisplay::showTimeEng(int hour, int min) {

    clearAll();

    bool showNext = false;

    show(IT);
    show(IS);

    // percek beallitasa
    // a percek / negyed orak
    if (min < 5) {
        show(OCLOCK);
    }
    else if (min < 10) {
        show(FIVE_MIN);
        show(PAST); // ora, mult, ot, perccel
    }
    else if (min < 15) {
        show(TEN_MIN);
        show(PAST);
    }
    else if (min < 20) {
        show(QUARTER);
        show(PAST);
    }
    else if (min < 25) {
        show(TWENTY);
        show(PAST);
    }
    else if (min < 30) {
        show(TWENTY);
        show(FIVE_MIN);
        show(PAST);
    }
    else if (min < 35) {
        show(HALF);
        show(PAST);
    }
    else if (min < 40) {
        show(TWENTY);
        show(FIVE_MIN);  // five to
        show(TO);
        showNext = true;
    }
    else if (min < 45) {
        show(TWENTY);
        show(TO);
        showNext = true;
    }
    else if (min < 50) {
        show(QUARTER);
        show(TO);
        showNext = true;
    }
    else if (min < 55) {
        show(TEN_MIN);
        show(TO);
        showNext = true;
    }
    else {
        show(FIVE_MIN);
        show(TO);
        showNext = true;
    }

    if (showNext) {
        hour = hour + 1;
    }

    // csak 12 orat jelzunk, a nulla is 12
    if (hour == 0) { hour = 12; }
    else if (hour > 12) { hour -= 12; }

    show(HOURS[hour - 1]);

    pixels.show();
}

// =================  debugging functions  =================

//Print an integer in "00" format (with leading zero).
//Input value assumed to be between 0 and 99.
void WordclockDisplay::sPrintI00(int val) {
    if (val < 10) Serial.print('0');
    Serial.print(val, DEC);
}

//Function to print time with time zone
void WordclockDisplay::debugPrintTime(time_t t, const char *tz) {
    sPrintI00(hour(t));
    Serial.print(':');
    sPrintI00(minute(t));
    Serial.print(':');
    sPrintI00(second(t));
    Serial.print(' ');
    Serial.print(dayShortStr(weekday(t)));
    Serial.print(' ');
    sPrintI00(day(t));
    Serial.print(' ');
    Serial.print(monthShortStr(month(t)));
    Serial.print(' ');
    Serial.print(year(t));
    Serial.print(' ');
    Serial.print(tz);
    Serial.println();
}
