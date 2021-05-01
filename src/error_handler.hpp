#ifndef _ERROR_HANDLER_H_
#define _ERROR_HANDLER_H_
#include <Arduino.h>

#define MAX_ERR_LEN 16
#define ERR_TPS_IMPLAUSIBLE 0
#define ERR_APPS_IMPLAUSIBLE 1
#define ERR_TPS_1_CIRCUIT_FAILURE 2
#define ERR_TPS_2_CIRCUIT_FAILURE 3
#define ERR_APPS_1_CIRCUIT_FAILURE 4
#define ERR_APPS_2_CIRCUIT_FAILURE 5
#define ERR_APPS_TPS_TARGET_FAILURE 6
class ErrorHandler
{
public:
    ErrorHandler();
    Errors errors;
    void raise(int8_t errID);
    void clear(int8_t errID);
    void clearAll();
    uint8_t errorsLength();
    String errorsToStr();

private:
    void initError(int8_t errID);
};

typedef Error Errors[MAX_ERR_LEN];

typedef struct Error
{
    const uint8_t ID;
    bool raised;
};

#endif