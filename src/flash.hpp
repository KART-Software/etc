#ifndef _FLASH_H_
#define _FLASH_H_

#include <FS.h>
#include <FFat.h>

#define FORMAT_FFAT_IF_FAILED true
#define FILE_NAME "/sensor_values.txt"

class Flash
{
public:
    Flash();
    void initialize();
    void write(const char *jsonStr);
    const char *read();

private:
};

#endif