#include "flash.hpp"

bool Flash::initialize()
{
    for (int i = 0; i < BEGIN_FFAT_LIMIT_TIMES; i++)
    {
        if (FFat.begin(FORMAT_FFAT_IF_FAILED))
        {
            return true;
        }
    }
    Serial.println("\033[KFFat begin failed.");
    return false;
}

void Flash::write(const char *fileName, const char *jsonStr)
{
    File file = FFat.open(fileName, FILE_WRITE);
    file.print(jsonStr);
    file.close();
    Serial.printf("\033[K---- Saved to %s ----\n", fileName);
    Serial.println(jsonStr);
}

const char *Flash::read(const char *fileName)
{
    File file = FFat.open(fileName);
    if (!file || file.isDirectory())
    {
        return "";
    }
    int len = file.available();
    char *jsonStr = new char[len + 1];
    if (len)
    {
        file.readBytes(jsonStr, len);
    }
    file.close();
    jsonStr[len] = '\0';
    Serial.printf("\033[K---- Loaded from %s ----\n", fileName);
    Serial.println(jsonStr);
    return jsonStr;
}