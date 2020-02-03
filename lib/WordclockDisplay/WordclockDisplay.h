#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#ifndef WORDCLOCKDISPLAY_h
#define WORDCLOCKDISPLAY_h

// languages
#define HUNGARIAN 0
#define ENGLISH 1


class WordclockDisplay
{
	public:
		WordclockDisplay(byte data_pin, byte lang, byte numpixels);
    void setTime(int h, int m);
    void setNextColour();
		void debugPrintTime(unsigned long time, const char *tz);


	private:
    int currentHour;
    int currentMinute;
		void reDraw();
    void clearAll();
		void show(byte data[]);
    void showAll(byte data[]);
    void showTimeHun(int hour, int min);
    void showTimeEng(int hour, int min);
    volatile byte chosenColour;
    volatile uint32_t currentColour;
    uint32_t colours[];
		byte EEPROM_ADDR;
		void sPrintI00(int val);
};
#endif
