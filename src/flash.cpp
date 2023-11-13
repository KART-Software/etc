#include "flash.hpp"

Flash::Flash() {}

void Flash::initialize()
{
    SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
}

void Flash::write(const char *jsonStr)
{
    File file = SPIFFS.open(FILE_NAME, FILE_WRITE);
    file.print(jsonStr);
    file.close();
    Serial.println("---- Saved ----");
    Serial.println(jsonStr);
}

const char *Flash::read()
{
    File file = SPIFFS.open(FILE_NAME);
    int len = file.available();
    char *jsonStr = new char[len + 1];
    if (len)
    {
        file.readBytes(jsonStr, len);
    }
    file.close();
    jsonStr[len] = '\0';
    Serial.println("---- Loaded ----");
    return jsonStr;
}