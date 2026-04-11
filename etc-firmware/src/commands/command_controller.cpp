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
    auto *c = static_cast<CommandContainer *>(cntr);
    c->configurator.save();
    StaticJsonDocument<512> tmp;
    JsonObject data = tmp.to<JsonObject>();
    c->configurator.getConfigJson(data);
    SerialProtocol::sendResponse(id, true, data);
}

void setAppsMin(void *cntr, JsonDocument &doc, uint32_t id)
{
    auto *c = static_cast<CommandContainer *>(cntr);
    c->configurator.setAppsMin();
    StaticJsonDocument<128> tmp;
    JsonObject data = tmp.to<JsonObject>();
    data["apps1Min"] = c->configurator.config.sensorValues.apps1Min;
    data["apps2Min"] = c->configurator.config.sensorValues.apps2Min;
    data["ittrMin"] = c->configurator.config.sensorValues.ittrMin;
    SerialProtocol::sendResponse(id, true, data);
}

void setAppsMax(void *cntr, JsonDocument &doc, uint32_t id)
{
    auto *c = static_cast<CommandContainer *>(cntr);
    c->configurator.setAppsMax();
    StaticJsonDocument<128> tmp;
    JsonObject data = tmp.to<JsonObject>();
    data["apps1Max"] = c->configurator.config.sensorValues.apps1Max;
    data["apps2Max"] = c->configurator.config.sensorValues.apps2Max;
    data["ittrMax"] = c->configurator.config.sensorValues.ittrMax;
    SerialProtocol::sendResponse(id, true, data);
}

void setTpsMin(void *cntr, JsonDocument &doc, uint32_t id)
{
    auto *c = static_cast<CommandContainer *>(cntr);
    c->configurator.setTpsMin();
    StaticJsonDocument<128> tmp;
    JsonObject data = tmp.to<JsonObject>();
    data["tps1Min"] = c->configurator.config.sensorValues.tps1Min;
    data["tps2Min"] = c->configurator.config.sensorValues.tps2Min;
    SerialProtocol::sendResponse(id, true, data);
}

void setTpsMax(void *cntr, JsonDocument &doc, uint32_t id)
{
    auto *c = static_cast<CommandContainer *>(cntr);
    c->configurator.setTpsMax();
    StaticJsonDocument<128> tmp;
    JsonObject data = tmp.to<JsonObject>();
    data["tps1Max"] = c->configurator.config.sensorValues.tps1Max;
    data["tps2Max"] = c->configurator.config.sensorValues.tps2Max;
    SerialProtocol::sendResponse(id, true, data);
}

void setIdling(void *cntr, JsonDocument &doc, uint32_t id)
{
    auto *c = static_cast<CommandContainer *>(cntr);
    c->configurator.setIdling();
    StaticJsonDocument<64> tmp;
    JsonObject data = tmp.to<JsonObject>();
    data["idling"] = c->configurator.config.sensorValues.idling;
    SerialProtocol::sendResponse(id, true, data);
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
    StaticJsonDocument<256> tmp;
    JsonObject data = tmp.to<JsonObject>();
    data["apps"] = flags.apps;
    data["tps"] = flags.tps;
    data["apps1"] = flags.apps1;
    data["apps2"] = flags.apps2;
    data["tps1"] = flags.tps1;
    data["tps2"] = flags.tps2;
    data["target"] = flags.target;
    data["bps"] = flags.bps;
    data["bpsTps"] = flags.bpsTps;
    SerialProtocol::sendResponse(id, true, data);
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
    auto *c = static_cast<CommandContainer *>(cntr);
    c->configurator.revert();
    StaticJsonDocument<512> tmp;
    JsonObject data = tmp.to<JsonObject>();
    c->configurator.getConfigJson(data);
    SerialProtocol::sendResponse(id, true, data);
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
