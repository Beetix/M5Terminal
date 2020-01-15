#include "M5Stack.h"
#include "M5Terminal.h"

// The scrolling area must be a integral multiple of TEXT_HEIGHT
#define TEXT_HEIGHT 16 // Height of text to be printed and scrolled
#define TOP_FIXED_AREA 14 // Number of lines in top fixed area (lines counted from top of screen)
#define BOT_FIXED_AREA 0 // Number of lines in bottom fixed area (lines counted from bottom of screen)
#define YMAX 240 // Bottom of screen area

void M5Terminal::init() {
    yStart = 16;
    // yArea must be a integral multiple of TEXT_HEIGHT
    yArea = YMAX-TOP_FIXED_AREA-BOT_FIXED_AREA; 
	yDraw = 0;
	xPos = 0;
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextColor(GREEN, TFT_BLACK);
    setupScrollArea(0, 0);
}

size_t M5Terminal::write(uint8_t c) {
    if (c == '\n' || xPos>311) {
      xPos = 0;
      yDraw = scroll_line(); // It can take 13ms to scroll and blank 16 pixel lines
    }
    if (c > 31 && c < 128) {
      xPos += M5.Lcd.drawChar(c, xPos, yDraw, 2);
    }
    return 1;
}

// ##############################################################################################
// Call this function to scroll the display one text line
// ##############################################################################################
int M5Terminal::scroll_line() {
    int yTemp = yStart; // Store the old yStart, this is where we draw the next line
    M5.Lcd.fillRect(0,yStart,320,TEXT_HEIGHT, TFT_BLACK);
	// Change the top of the scroll area
  	yStart+=TEXT_HEIGHT;
    // The value must wrap around as the screen memory is a circular buffer
  	if (yStart >= YMAX) yStart = 0;
  	// Now we can scroll the display
  	scrollAddress(yStart);
  	return  yTemp;
}

// ##############################################################################################
// Setup a portion of the screen for vertical scrolling
// ##############################################################################################
// We are using a hardware feature of the display, so we can only scroll in portrait orientation
void M5Terminal::setupScrollArea(uint16_t tfa, uint16_t bfa) {
    M5.Lcd.writecommand(ILI9341_VSCRDEF); // Vertical scroll definition
    M5.Lcd.writedata(tfa >> 8);           // Top Fixed Area line count
    M5.Lcd.writedata(tfa);
    M5.Lcd.writedata((YMAX-tfa-bfa)>>8);  // Vertical Scrolling Area line count
    M5.Lcd.writedata(YMAX-tfa-bfa);
    M5.Lcd.writedata(bfa >> 8);           // Bottom Fixed Area line count
    M5.Lcd.writedata(bfa);
}

// ##############################################################################################
// Setup the vertical scrolling start address pointer
// ##############################################################################################
void M5Terminal::scrollAddress(uint16_t vsp) {
    M5.Lcd.writecommand(ILI9341_VSCRSADD); // Vertical scrolling pointer
    M5.Lcd.writedata(vsp>>8);
    M5.Lcd.writedata(vsp);
}
