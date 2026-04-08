#include "command_controller.hpp"

CommandController::CommandController(Configurator &configurator, MotorController &motorController, Target &target)
    : container{configurator, motorController, target}
{
}

void CommandController::registerCommands(CommandRouter &router)
{
    router.on("cal_start", onCalStart, &container);
    router.on("cal_finish", onCalFinish, &container);
    router.on("motor_off", onMotorOff, &container);
    router.on("set_apps_min", onSetAppsMin, &container);
    router.on("set_apps_max", onSetAppsMax, &container);
    router.on("set_tps_min", onSetTpsMin, &container);
    router.on("set_tps_max", onSetTpsMax, &container);
    router.on("set_idling", onSetIdling, &container);
    router.on("set_flag", onSetFlag, &container);
    router.on("set_ittr", onSetIttr, &container);
    router.on("set_manual", onSetManual, &container);
    router.on("manual_plus", onManualPlus, &container);
    router.on("manual_minus", onManualMinus, &container);
    router.on("get_config", onGetConfig, &container);
    router.on("set_config", onSetConfig, &container);
    router.on("reboot", onReboot, nullptr);
}

void CommandController::onCalStart(void *ctx, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(ctx)->configurator.startCalibration();
    SerialProtocol::sendResponse(id, true);
}

void CommandController::onCalFinish(void *ctx, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(ctx)->configurator.finishCalibration();
    SerialProtocol::sendResponse(id, true);
}

void CommandController::onMotorOff(void *ctx, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(ctx)->motorController.setMotorOff();
    SerialProtocol::sendResponse(id, true);
}

void CommandController::onSetAppsMin(void *ctx, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(ctx)->configurator.setAppsMin();
    SerialProtocol::sendResponse(id, true);
}

void CommandController::onSetAppsMax(void *ctx, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(ctx)->configurator.setAppsMax();
    SerialProtocol::sendResponse(id, true);
}

void CommandController::onSetTpsMin(void *ctx, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(ctx)->configurator.setTpsMin();
    SerialProtocol::sendResponse(id, true);
}

void CommandController::onSetTpsMax(void *ctx, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(ctx)->configurator.setTpsMax();
    SerialProtocol::sendResponse(id, true);
}

void CommandController::onSetIdling(void *ctx, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(ctx)->configurator.setIdling();
    SerialProtocol::sendResponse(id, true);
}

void CommandController::onSetFlag(void *ctx, JsonDocument &doc, uint32_t id)
{
    auto *c = static_cast<CommandContainer *>(ctx);
    const char *key = doc["d"]["key"];
    bool val = doc["d"]["val"];
    bool found = c->configurator.setPlausibilityFlag(key, val);
    SerialProtocol::sendResponse(id, found);
}

void CommandController::onSetIttr(void *ctx, JsonDocument &doc, uint32_t id)
{
    bool val = doc["d"]["val"];
    static_cast<CommandContainer *>(ctx)->configurator.setIttrFlag(val);
    SerialProtocol::sendResponse(id, true);
}

void CommandController::onSetManual(void *ctx, JsonDocument &doc, uint32_t id)
{
    bool ok = static_cast<CommandContainer *>(ctx)->target.setManual();
    SerialProtocol::sendResponse(id, ok);
}

void CommandController::onManualPlus(void *ctx, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(ctx)->target.manualPlus();
    SerialProtocol::sendResponse(id, true);
}

void CommandController::onManualMinus(void *ctx, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(ctx)->target.manualMinus();
    SerialProtocol::sendResponse(id, true);
}

void CommandController::onGetConfig(void *ctx, JsonDocument &doc, uint32_t id)
{
    StaticJsonDocument<512> resp;
    JsonObject data = resp.to<JsonObject>();
    static_cast<CommandContainer *>(ctx)->configurator.getConfigJson(data);
    SerialProtocol::sendResponse(id, true, data);
}

void CommandController::onSetConfig(void *ctx, JsonDocument &doc, uint32_t id)
{
    const char *jsonStr = doc["d"]["config"];
    bool ok = jsonStr && static_cast<CommandContainer *>(ctx)->configurator.importConfig(jsonStr);
    SerialProtocol::sendResponse(id, ok);
}

void CommandController::onReboot(void *ctx, JsonDocument &doc, uint32_t id)
{
    SerialProtocol::sendResponse(id, true);
    delay(100);
    SCB_AIRCR = 0x05FA0004; // Teensy software reset
}
