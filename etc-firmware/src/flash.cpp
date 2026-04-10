#include "flash.hpp"
#include "serial_protocol.hpp"

bool Flash::initialize()
{
    for (int i = 0; i < BEGIN_FS_LIMIT_TIMES; i++)
    {
        if (fs.begin(256 * 1024))
        {
            return true;
        }
    }
    SerialProtocol::sendDebug("LittleFS begin failed.");
    return false;
}

void Flash::write(const char *fileName, const String &jsonStr)
{
    File file = fs.open(fileName, FILE_WRITE);
    file.print(jsonStr);
    file.close();
    char buf[128];
    snprintf(buf, sizeof(buf), "Saved to %s", fileName);
    SerialProtocol::sendDebug(buf);
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
    char buf[128];
    snprintf(buf, sizeof(buf), "Loaded from %s", fileName);
    SerialProtocol::sendDebug(buf);
    return jsonStr;
}