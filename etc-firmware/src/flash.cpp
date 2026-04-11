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
                SerialProtocol::sendDebugf("LittleFS nearly full, formatting...");
                fs.quickFormat();
            }
            return true;
        }
    }
    SerialProtocol::sendDebugf("LittleFS begin failed.");
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
        SerialProtocol::sendDebugf("Flash write: open failed");
        return;
    }
    size_t written = file.write((const uint8_t *)jsonStr.c_str(), jsonStr.length());
    file.close();
    SerialProtocol::sendDebugf("Saved to %s (%u bytes)", fileName, (unsigned)written);
}

String Flash::read(const char *fileName)
{
    File file = fs.open(fileName);
    if (!file || file.isDirectory())
    {
        SerialProtocol::sendDebugf("Flash read: %s not found", fileName);
        return String();
    }
    String jsonStr = file.readString();
    file.close();
    SerialProtocol::sendDebugf("Loaded from %s (%u bytes)", fileName, (unsigned)jsonStr.length());
    return jsonStr;
}

void Flash::remove(const char *fileName)
{
    fs.remove(fileName);
}