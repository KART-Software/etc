#include "error_handler.hpp"

ErrorHandler::
    ErrorHandler()
    : errors(
          {{ERR_TPS_IMPLAUSIBLE, false},
           {ERR_APPS_IMPLAUSIBLE, false},
           {ERR_TPS_1_CIRCUIT_FAILURE, false},
           {ERR_TPS_1_CIRCUIT_FAILURE, false}})
{
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