#include "command_controller.hpp"

namespace
{

void motorOff(void *cntr, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(cntr)->motorController.setMotorOff();
    SerialProtocol::sendResponse(id, true);
}

void save(void *cntr, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(cntr)->configurator.save();
    SerialProtocol::sendResponse(id, true);
}

void setAppsMin(void *cntr, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(cntr)->configurator.setAppsMin();
    SerialProtocol::sendResponse(id, true);
}

void setAppsMax(void *cntr, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(cntr)->configurator.setAppsMax();
    SerialProtocol::sendResponse(id, true);
}

void setTpsMin(void *cntr, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(cntr)->configurator.setTpsMin();
    SerialProtocol::sendResponse(id, true);
}

void setTpsMax(void *cntr, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(cntr)->configurator.setTpsMax();
    SerialProtocol::sendResponse(id, true);
}

void setIdling(void *cntr, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(cntr)->configurator.setIdling();
    SerialProtocol::sendResponse(id, true);
}

void setPlausibilityCheckFlags(void *cntr, JsonDocument &doc, uint32_t id)
{
    auto *c = static_cast<CommandContainer *>(cntr);
    JsonObject d = doc["d"];
    PlausibilityCheckFlags flags;
    flags.apps = d["apps"] | false;
    flags.tps = d["tps"] | false;
    flags.apps1 = d["apps1"] | false;
    flags.apps2 = d["apps2"] | false;
    flags.tps1 = d["tps1"] | false;
    flags.tps2 = d["tps2"] | false;
    flags.target = d["target"] | false;
    flags.bps = d["bps"] | false;
    flags.bpsTps = d["bpsTps"] | false;
    c->configurator.setPlausibilityFlags(flags);
    SerialProtocol::sendResponse(id, true);
}

void setIttr(void *cntr, JsonDocument &doc, uint32_t id)
{
    bool val = doc["d"]["val"];
    static_cast<CommandContainer *>(cntr)->configurator.setIttrFlag(val);
    SerialProtocol::sendResponse(id, true);
}

void setManual(void *cntr, JsonDocument &doc, uint32_t id)
{
    bool ok = static_cast<CommandContainer *>(cntr)->target.setManual();
    SerialProtocol::sendResponse(id, ok);
}

void manualAdjust(void *cntr, JsonDocument &doc, uint32_t id)
{
    double amount = doc["d"]["amount"];
    static_cast<CommandContainer *>(cntr)->target.manualAdjust(amount);
    SerialProtocol::sendResponse(id, true);
}

void getConfig(void *cntr, JsonDocument &doc, uint32_t id)
{
    StaticJsonDocument<512> resp;
    JsonObject data = resp.to<JsonObject>();
    static_cast<CommandContainer *>(cntr)->configurator.getConfigJson(data);
    SerialProtocol::sendResponse(id, true, data);
}

void setConfig(void *cntr, JsonDocument &doc, uint32_t id)
{
    const char *jsonStr = doc["d"]["config"];
    bool ok = jsonStr && static_cast<CommandContainer *>(cntr)->configurator.importConfig(jsonStr);
    SerialProtocol::sendResponse(id, ok);
}

void reboot(void *cntr, JsonDocument &doc, uint32_t id)
{
    SerialProtocol::sendResponse(id, true);
    delay(100);
    SCB_AIRCR = 0x05FA0004; // Teensy software reset
}

void revert(void *cntr, JsonDocument &doc, uint32_t id)
{
    static_cast<CommandContainer *>(cntr)->configurator.revert();
    SerialProtocol::sendResponse(id, true);
}

} // namespace

CommandController::CommandController(Configurator &configurator, MotorController &motorController, Target &target)
    : container{configurator, motorController, target}
{
}

void CommandController::registerCommands(CommandRouter &router)
{
    router.on("motor_off", motorOff, &container);
    router.on("save", save, &container);
    router.on("set_apps_min", setAppsMin, &container);
    router.on("set_apps_max", setAppsMax, &container);
    router.on("set_tps_min", setTpsMin, &container);
    router.on("set_tps_max", setTpsMax, &container);
    router.on("set_idling", setIdling, &container);
    router.on("set_plausibility_check_flags", setPlausibilityCheckFlags, &container);
    router.on("set_ittr", setIttr, &container);
    router.on("set_manual", setManual, &container);
    router.on("manual_adjust", manualAdjust, &container);
    router.on("get_config", getConfig, &container);
    router.on("set_config", setConfig, &container);
    router.on("reboot", reboot, nullptr);
    router.on("revert", revert, &container);
}
