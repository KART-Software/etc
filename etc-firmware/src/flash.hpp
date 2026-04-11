#ifndef _FLASH_H_
#define _FLASH_H_

#include <LittleFS.h>

#define BEGIN_FS_LIMIT_TIMES 5

class Flash
{
public:
    bool initialize();
    void write(const char *fileName, const String &jsonStr);
    String read(const char *fileName);
    void remove(const char *fileName);

private:
    LittleFS_Program fs;
};

#endif