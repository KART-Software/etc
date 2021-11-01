#ifndef _LCD_LOGGER_H_
#define _LCD_LOGGER_H_

#include <M5Stack.h>
#include "globals.hpp"

#define LCD_UPDATE_PER_CYCLES 50

#define LINE_COLOR WHITE

#define ERRORS_LENGTH 7
#define ERROR_COLOR RED
#define NON_ERROR_COLOR BLACK

class LcdIndicator
{
public:
    LcdIndicator();
    virtual void initialize();
    virtual void setGui();
    virtual void update();

private:
    const int16_t lcdWidth = 320;
    const int16_t lcdHeight = 240;
    const String footerString[3];
    const int16_t footerSeparatorX[2] = {106, 214};
    const int16_t footerCursorX[3] = {0, 106, 214};
    const int16_t footerCursorY = 208;
    const int16_t footerHeight = 32;
    void drawFooter();
};

class ErrorIndicator : LcdIndicator
{
public:
    ErrorIndicator();
    void update();
    void initialize();

private:
    const String string[ERRORS_LENGTH] = {"TPS IMPLS",
                                          "APPS IMPLS",
                                          "TPS1 CIRCT",
                                          "TPS2 CIRCT",
                                          "APPS1 CIRCT",
                                          "APPS2 CIRCT",
                                          "APP TP TRGT"};
    const int16_t cursorX[ERRORS_LENGTH] = {0, 160, 0, 160, 0, 160, 0};
    const int16_t cursorY[ERRORS_LENGTH] = {0, 0, 52, 52, 104, 104, 156};
    const int16_t cursorShiftY = 19;
    const int16_t cellWidth = 160;
    const int16_t cellHeight = 52;
    const String footerString[3] = {"STOP", "<-LONG PRESS->", "Adjuster Mode"};
    void raise(int errorID);
    void normalizeAll();
    void normalize(int errorID);
};

class ManualParameterAdjusterInterface : LcdIndicator
{
public:
    ManualParameterAdjusterInterface();

private:
};

#endif
