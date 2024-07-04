#ifndef _FLASH_H_
#define _FLASH_H_

#include <FS.h>
#include <FFat.h>

#define FORMAT_FFAT_IF_FAILED true
#define BEGIN_FFAT_LIMIT_TIMES 5

class Flash
{
public:
    bool initialize();
    void write(const char *fileName, const char *jsonStr);
    const char *read(const char *fileName);

private:
};

#endif