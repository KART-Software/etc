#ifndef _FLASH_H_
#define _FLASH_H_

#include <LittleFS.h>

#define BEGIN_FS_LIMIT_TIMES 5

class Flash
{
public:
    bool initialize();
    void write(const char *fileName, const char *jsonStr);
    const char *read(const char *fileName);

private:
    LittleFS_Program fs;
};

#endif