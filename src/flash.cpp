#include "flash.hpp"

bool Flash::initialize()
{
    for (int i = 0; i < BEGIN_FS_LIMIT_TIMES; i++)
    {
        if (fs.begin(256 * 1024))
        {
            return true;
        }
    }
    Serial.println("\033[KLittleFS begin failed.");
    return false;
}

void Flash::write(const char *fileName, const String &jsonStr)
{
    File file = fs.open(fileName, FILE_WRITE);
    file.print(jsonStr);
    file.close();
    Serial.printf("\033[K---- Saved to %s ----\n", fileName);
    Serial.println(jsonStr);
}

String Flash::read(const char *fileName)
{
    File file = fs.open(fileName);
    if (!file || file.isDirectory())
    {
        return String();
    }
    String jsonStr = file.readString();
    file.close();
    Serial.printf("\033[K---- Loaded from %s ----\n", fileName);
    Serial.println(jsonStr);
    return jsonStr;
}