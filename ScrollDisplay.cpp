#include "ScrollDisplay.h"


// Initialises the display and clears it
void ScrollDisplay::setup( uint8_t displayAddress ) {
	line1[0] = line2[0] = line3[0] = line4[0] = 0;

	begin( SSD1306_SWITCHCAPVCC, displayAddress );
	clearDisplay();
	display();
}


// Should be called constantly from main loop
void ScrollDisplay::handle() {
	if ( blanking && millis() - blankStarted > (long) blanktime * 1000 ) {
		blanking = false;
		for ( int i = 0; i < 4; i++ ) printScrollLine( "" );
	}
}


// This sets the time that the display will be dimmed
void ScrollDisplay::blankAfter( uint16_t blankTime ) {
	blankStarted = millis();
	this->blanktime = blankTime;
	blanking = true;
}

// Scrolls up the existing lines in display and prints a new line in the buttom.
void ScrollDisplay::printScrollLine( char* line ) {
	scrollLinesUp();
	strncpy( line4, line, 19 );

	clearDisplay();
	setTextSize( 1 );
	setTextWrap( true );
	setTextColor( WHITE );
	setCursor( 0, 0 );

	println( line1 );
	println( line2 );
	println( line3 );
	println( line4 );
	
	display();
}


// Rotates all lines one position up.
void ScrollDisplay::scrollLinesUp() {
	strncpy( line1, line2, 19 );
	strncpy( line2, line3, 19 );
	strncpy( line3, line4, 19 );
}

