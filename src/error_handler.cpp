#include "error_handler.hpp"

ErrorHandler::
    ErrorHandler()
    : errors(
          {
              {ERR_TPS_IMPLAUSIBLE, false},
              {ERR_APPS_IMPLAUSIBLE, false},
              {ERR_TPS_1_CIRCUIT_FAILURE, false},
              {ERR_TPS_2_CIRCUIT_FAILURE, false},
              {ERR_APPS_1_CIRCUIT_FAILURE, false},
              {ERR_APPS_2_CIRCUIT_FAILURE, false},
              {ERR_APPS_TPS_TARGET_FAILURE, false},
              {ERR_BPS_CIRCUIT_FAILURE, false},
              {ERR_BPS_TPS_IMPLAUSIBLE, false},
          }){};

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

bool ErrorHandler::raised(int8_t errID)
{
    return errors[errID].raised;
}