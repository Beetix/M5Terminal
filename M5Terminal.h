#ifndef TERMINAL_H
#define TERMINAL_H

#include "Print.h"

class M5Terminal : public Print {
    public:
        void init();
        virtual size_t write(uint8_t c);
    private:
        // The initial y coordinate of the top of the scrolling area
        uint16_t yStart;
        uint16_t yArea;
        // The initial y coordinate of the top of the bottom text line
        uint16_t yDraw;
        // Keep track of the drawing x coordinate
		uint16_t xPos;
        int scroll_line();
        void setupScrollArea(uint16_t tfa, uint16_t bfa);
        void scrollAddress(uint16_t vsp);
};
#endif
