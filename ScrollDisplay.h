#ifndef _SCROLLDISPLAY_h
#define _SCROLLDISPLAY_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


class ScrollDisplay : public Adafruit_SSD1306
{

protected:
	char line1[20];
	char line2[20];
	char line3[20];
	char line4[20];
	void scrollLinesUp();
	uint16_t blanktime;
	long blankStarted;
	bool blanking = false;

 public:
	ScrollDisplay( uint8_t x ) : Adafruit_SSD1306( x ) {};
	void setup( uint8_t displayAddress );
	void handle();
	void printScrollLine( char* line );
	void blankAfter( uint16_t blanktime );
};


#endif

