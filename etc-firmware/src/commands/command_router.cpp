#include "command_router.hpp"
#include <string.h>

void CommandRouter::on(const char *command, CommandHandler handler, void *ctx)
{
    if (routeCount < MAX_ROUTES)
    {
        routes[routeCount++] = {command, handler, ctx};
    }
}

void CommandRouter::poll()
{
    StaticJsonDocument<COMMAND_JSON_SIZE> doc;
    if (!SerialProtocol::readCommand(doc))
    {
        return;
    }

    const char *cmd = doc["c"];
    uint32_t id = doc["id"] | 0;

    for (uint8_t i = 0; i < routeCount; i++)
    {
        if (strcmp(cmd, routes[i].command) == 0)
        {
            routes[i].handler(routes[i].ctx, doc, id);
            return;
        }
    }

    // No route matched
    SerialProtocol::sendResponse(id, false);
}
