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

    static void onCalStart(void *ctx, JsonDocument &doc, uint32_t id);
    static void onCalFinish(void *ctx, JsonDocument &doc, uint32_t id);
    static void onMotorOff(void *ctx, JsonDocument &doc, uint32_t id);
    static void onSetAppsMin(void *ctx, JsonDocument &doc, uint32_t id);
    static void onSetAppsMax(void *ctx, JsonDocument &doc, uint32_t id);
    static void onSetTpsMin(void *ctx, JsonDocument &doc, uint32_t id);
    static void onSetTpsMax(void *ctx, JsonDocument &doc, uint32_t id);
    static void onSetIdling(void *ctx, JsonDocument &doc, uint32_t id);
    static void onSetFlag(void *ctx, JsonDocument &doc, uint32_t id);
    static void onSetIttr(void *ctx, JsonDocument &doc, uint32_t id);
    static void onSetManual(void *ctx, JsonDocument &doc, uint32_t id);
    static void onManualPlus(void *ctx, JsonDocument &doc, uint32_t id);
    static void onManualMinus(void *ctx, JsonDocument &doc, uint32_t id);
    static void onGetConfig(void *ctx, JsonDocument &doc, uint32_t id);
    static void onSetConfig(void *ctx, JsonDocument &doc, uint32_t id);
    static void onReboot(void *ctx, JsonDocument &doc, uint32_t id);
};

#endif
