#include "flash.hpp"

Flash::Flash() {}

void Flash::initialize()
{
    if (!FFat.begin(FORMAT_FFAT_IF_FAILED))
    {
        // TODO Failのときの処理
        Serial.println("FFat begin failed.");
    }
}

void Flash::write(const char *jsonStr)
{
    File file = FFat.open(FILE_NAME, FILE_WRITE, true);
    file.print(jsonStr);
    file.close();
    Serial.println("---- Saved ----");
    Serial.println(jsonStr);
}

const char *Flash::read()
{
    File file = FFat.open(FILE_NAME);
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