#pragma once

#include "./library/events/events.h"
#include "./library/requests/requests.h"
#include "./library/parsing/parse.h"
#include <functional>

namespace MessageEventsAPI {
    void setEventListener(std::function<void(Event const &)> callback);
}

extern char prefix;

extern bool messageNotifications;

int runClient(std::string & botToken);

bool isCommand(std::string & string);
