#include "error_handler.hpp"

ErrorHandler::
    ErrorHandler()
{
    initError(ERR_TPS_IMPLAUSIBLE);
    initError(ERR_APPS_IMPLAUSIBLE);
    initError(ERR_TPS_1_CIRCUIT_FAILURE);
    initError(ERR_TPS_2_CIRCUIT_FAILURE);
    initError(ERR_APPS_1_CIRCUIT_FAILURE);
    initError(ERR_APPS_2_CIRCUIT_FAILURE);
    initError(ERR_APPS_TPS_TARGET_FAILURE);
}

void ErrorHandler::initError(int8_t errID)
{
    errors[errID] = {errID, false};
}

void ErrorHandler::raise(int8_t errID)
{
    for (int i; i < errorsLength(); i++)
    {
        if (errors[i].ID == errID)
        {
            errors[i].raised = true;
        }
    }
}

void ErrorHandler::clear(int8_t errID)
{
    for (int i; i < errorsLength(); i++)
    {
        if (errors[i].ID == errID)
        {
            errors[i].raised = false;
        }
    }
}

void ErrorHandler::clearAll()
{
    for (int i; i < errorsLength(); i++)
    {
        errors[i].raised = false;
    }
}

uint8_t ErrorHandler::errorsLength()
{
    return sizeof(errors) / sizeof(Error);
}

String ErrorHandler::errorsToStr()
{
    String errStr = "";
    for (int i = 0; i < MAX_ERR_LEN; i++)
    {
        if (errors[i]->raised)
        {
            errStr += String(i);
            if (i != MAX_ERR_LEN - 1)
            {
                errStr += ", ";
            }
        }
    }
    return errStr;
}