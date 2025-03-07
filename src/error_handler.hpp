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
#define ERR_BPS_CIRCUIT_FAILURE 7
#define ERR_BPS_TPS_IMPLAUSIBLE 8

struct Error
{
    const uint8_t ID;
    bool raised;
};

typedef Error Errors[MAX_ERR_LEN];
class ErrorHandler
{
public:
    ErrorHandler();
    Errors errors;
    void raise(int8_t errID);
    void clear(int8_t errID);
    void clearAll();
    uint8_t errorsLength();
    bool raised(int8_t errID);

private:
    void initError(int8_t errID);
};

#endif