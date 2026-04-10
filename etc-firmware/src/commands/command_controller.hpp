#ifndef _COMMAND_CONTROLLER_H_
#define _COMMAND_CONTROLLER_H_

#include "command_router.hpp"
#include "../configurator.hpp"
#include "../motor_controller.hpp"
#include "../sensors.hpp"

struct CommandContainer
{
    Configurator &configurator;
    MotorController &motorController;
    Target &target;
};

class CommandController
{
public:
    CommandController(Configurator &configurator, MotorController &motorController, Target &target);
    void registerCommands(CommandRouter &router);

private:
    CommandContainer container;
};

#endif
