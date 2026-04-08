#ifndef _COMMAND_ROUTER_H_
#define _COMMAND_ROUTER_H_

#include <ArduinoJson.h>
#include "../serial_protocol.hpp"

#define MAX_ROUTES 24

using CommandHandler = void (*)(void *ctx, JsonDocument &doc, uint32_t id);

struct Route
{
    const char *command;
    CommandHandler handler;
    void *ctx;
};

class CommandRouter
{
public:
    void on(const char *command, CommandHandler handler, void *ctx = nullptr);
    void poll();

private:
    Route routes[MAX_ROUTES];
    uint8_t routeCount = 0;
};

#endif
