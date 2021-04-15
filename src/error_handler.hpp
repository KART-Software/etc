#ifndef _ERROR_HANDLER_H_
#define _ERROR_HANDLER_H_
#include <Arduino.h>

#define MAX_ERR_LEN 16
#define ERR_TPS_IMPLAUSIBLE 1
#define ERR_APPS_IMPLAUSIBLE 2

class ErrorHandler
{
public:
    ErrorHandler();
    Errors errors;
    void raise(int8_t errID);
    void clear(int8_t errID);
    void clearAll();
    uint8_t errorsLength();
};

typedef Error Errors[MAX_ERR_LEN];

typedef struct Error
{
    const uint8_t ID;
    bool raised;
};

#endif