#include "flash.hpp"
#include "serial_protocol.hpp"

bool Flash::initialize()
{
    for (int i = 0; i < BEGIN_FS_LIMIT_TIMES; i++)
    {
        if (fs.begin(256 * 1024))
        {
            // 使用率が90%超の場合はフォーマット
            if (fs.totalSize() > 0 && fs.usedSize() > fs.totalSize() * 9 / 10)
            {
                SerialProtocol::sendDebug("LittleFS nearly full, formatting...");
                fs.quickFormat();
            }
            return true;
        }
    }
    SerialProtocol::sendDebug("LittleFS begin failed.");
    return false;
}

void Flash::write(const char *fileName, const String &jsonStr)
{
    if (fs.exists(fileName))
    {
        fs.remove(fileName);
    }
    File file = fs.open(fileName, FILE_WRITE);
    if (!file)
    {
        SerialProtocol::sendDebug("Flash write: open failed");
        return;
    }
    size_t written = file.write((const uint8_t *)jsonStr.c_str(), jsonStr.length());
    file.close();
    char buf[128];
    snprintf(buf, sizeof(buf), "Saved to %s (%u bytes)", fileName, (unsigned)written);
    SerialProtocol::sendDebug(buf);
}

String Flash::read(const char *fileName)
{
    File file = fs.open(fileName);
    if (!file || file.isDirectory())
    {
        char buf[128];
        snprintf(buf, sizeof(buf), "Flash read: %s not found", fileName);
        SerialProtocol::sendDebug(buf);
        return String();
    }
    String jsonStr = file.readString();
    file.close();
    char buf[128];
    snprintf(buf, sizeof(buf), "Loaded from %s (%u bytes)", fileName, (unsigned)jsonStr.length());
    SerialProtocol::sendDebug(buf);
    return jsonStr;
}

void Flash::remove(const char *fileName)
{
    fs.remove(fileName);
}