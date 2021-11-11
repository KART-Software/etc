#ifndef _LCD_LOGGER_H_
#define _LCD_LOGGER_H_

#include <M5Stack.h>
#include "globals.hpp"
#include "servo_controller.hpp"

#define LCD_UPDATE_PER_CYCLES 50
#define ADJUSTER_INTERFACE_UPDATE_PER_MS 100

#define LINE_COLOR WHITE

#define ERRORS_LENGTH 7
#define ERROR_COLOR RED
#define NON_ERROR_COLOR BLACK

class LcdIndicator
{
public:
    LcdIndicator();
    void initialize();
    void drawFooter(const String footerString[]);
    virtual void setGui(){};
    virtual void update(){};

private:
    const int16_t lcdWidth = 320;
    const int16_t lcdHeight = 240;
    const String footerString[3];
    const int8_t footerTextSize = 1;
    const int16_t footerSeparatorX[2] = {106, 214};
    const int16_t footerCursorX[3] = {0, 106, 214};
    const int16_t footerCursorY = 208;
    const int16_t footerCursorShiftY = 10;
    const int16_t footerHeight = 32;
};

class ErrorIndicator : public LcdIndicator
{
public:
    ErrorIndicator();
    void update();
    void initialize();

private:
    const int8_t textSize = 2;
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
    const String footerString[3] = {"STOP", "Clear Errors", "Adjuster Mode"};
    void raise(int errorID);
    void normalizeAll();
    void normalize(int errorID);
};

class ManualParameterAdjusterInterface : LcdIndicator
{
public:
    ManualParameterAdjusterInterface(ServoController *servoController);
    void update();
    void initialize();

private:
    const int8_t textSize = 2;
    const String stringRow[4] = {"APPS1", "APPS2", "TPS1", "TPS2"};
    const int16_t coodinatesX[4] = {0, 80, 160, 240};
    const int16_t coodinatesY[5] = {0, 41, 82, 123, 164};
    const int16_t cellWidth = 80;
    const int16_t cellHeight = 41;
    const int16_t cursorShiftY = 15;
    const String stringColumn[2] = {"RAW", "CONV"};
    const String stringServo = "SERVO";
    const String footerString[3] = {"UP", "DOWN", "FINISH"};
    void drawTitles();
    ServoController *servoController;
};

#endif
