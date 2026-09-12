#include "./events.h"
#include "../networking/networking.h"

void StoatSession::handleEvent(const json & data) {
    std::string type = data.value("type", "");

    if (type == "Authenticated") {
        authenticated = true;
        startHeartbeat();
        return;
    }

    if (type == "Ready") {
        if (notifications) std::cout << "bot started" << '\n';
        return;
    }

    if (type == "Pong") {
        if (notifications) std::cout << "booped bak" << '\n';
        return;
    }

    if (type == "Error") {
        std::string error = data.value("error", "unknown");
        std::cerr << "[Gateway ERROR] " << error << '\n';
        return;
    }

    if (type == "Logout") {
        std::cerr << "[Gateway] Bot token/session was logged out." << '\n';
        return;
    }

    if (type != "ChannelStopTyping" && type != "ChannelStartTyping") {
        messageEventsObject.onMessage(data, type);
        // std::cout << data.dump(4) << '\n';
    } else if (type == "ChannelStopTyping" || type == "ChannelStartTyping") {
        messageEventsObject.onMessage(data, type);
    }

    // std::cout << "event: " << type << '\n';
    // std::cout << data.dump(4) << '\n';
}

bool isCommand(std::string & string) {
    return (string[0] == messageEventsObject.prefix_);
}
