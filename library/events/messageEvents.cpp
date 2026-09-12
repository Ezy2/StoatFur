#include "./events.h"
#include "../networking/networking.h"
#include "../includes.h"
#include "../../stoatFur.h"
#include "../parsing/parse.h"

MessageEvents messageEventsObject(prefix, messageNotifications);

namespace MessageEventsAPI {
    void setEventListener(messageCallBack_ callback) {
        messageEventsObject.setEventListener(std::move(callback));
    }
}

MessageEvents::MessageEvents(char prefix, bool messageNotifications) : prefix_(std::move(prefix)), notifications(std::move(messageNotifications)) {}

void MessageEvents::run() {
    if (messageNotifications) {
        std::cout << "MessageEvents setup" << '\n';
    }
}

void MessageEvents::onMessage(json messageData, std::string type) {
    Event event;

    parseMessageData(messageData, event.message, type);
    parseAuthorData(messageData, event.author, type);
    parseReactedData(messageData, event.emoji, type);

    event.rawData = messageData.dump(4);
    event.type = type;

    if (messageListener) messageListener(event);
}

MessageInfo MessageEvents::getMessage() {
    return MessageInfo_;
}

void MessageEvents::setEventListener(messageCallBack_ callback) {
    messageListener = std::move(callback);
}

void getArguments(std::vector<CommandArgument> & vector, std::string string) {
    CommandArgument commandArgument;

    for (char const character : string) {
        if (character != ' ') {
            commandArgument.argument += character;
        } else {
            vector.push_back(commandArgument);
            commandArgument.argument = "";
        }
    }
    vector.push_back(commandArgument);
    commandArgument.argument = "";
}
