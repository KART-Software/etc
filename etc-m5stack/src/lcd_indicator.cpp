#include "lcd_indicator.hpp"

LcdIndicator::LcdIndicator()
{
}

void LcdIndicator::initialize()
{
}

void LcdIndicator::drawFooter(String footerString[])
{
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.drawRect(0, footerCursorY, lcdWidth, lcdHeight, LINE_COLOR);
    M5.Lcd.drawFastVLine(footerSeparatorX[0], footerCursorY, footerHeight, LINE_COLOR);
    M5.Lcd.drawFastVLine(footerSeparatorX[1], footerCursorY, footerHeight, LINE_COLOR);
    M5.Lcd.setTextSize(footerTextSize);
    for (int i = 0; i < 3; i++)
    {
        M5.Lcd.setCursor(footerCursorX[i], footerCursorY + footerCursorShiftY);
        M5.Lcd.print(footerString[i]);
    }
}

ErrorIndicator::ErrorIndicator()
{
}

void ErrorIndicator::initialize()
{
    drawFooter(footerString);
    normalizeAll();
}

void ErrorIndicator::update()
{
    for (int i = 0; i < ERRORS_LENGTH; i++)
    {
        if (gErrorHandler.errors[i].raised)
        {
            this->raise(i);
        }
        else
        {
            this->normalize(i);
        }
    }
}

void ErrorIndicator::normalizeAll()
{
    for (int i = 0; i < ERRORS_LENGTH; i++)
    {
        normalize(i);
    }
}

void ErrorIndicator::normalize(int errorID)
{
    M5.Lcd.fillRect(cursorX[errorID], cursorY[errorID], cellWidth, cellHeight, BLACK);
    M5.Lcd.setCursor(cursorX[errorID], cursorY[errorID] + cursorShiftY);
    M5.Lcd.setTextSize(textSize);
    M5.Lcd.print(string[errorID]);
}

void ErrorIndicator::raise(int errorID)
{
    M5.Lcd.fillRect(cursorX[errorID], cursorY[errorID], cellWidth, cellHeight, RED);
    M5.Lcd.setCursor(cursorX[errorID], cursorY[errorID] + cursorShiftY);
    M5.Lcd.setTextSize(textSize);
    M5.Lcd.print(string[errorID]);
}