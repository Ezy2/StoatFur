#pragma once

#include <functional>
#include <utility>
#include <string>
#include <variant>

#include "../includes.h"
#include "../parsing/parse.h"

using json = nlohmann::json;
using messageCallBack_ = std::function<void(Event const &)>;

struct MessageEvents : public std::enable_shared_from_this<MessageEvents> {
    MessageEvents(char prefix, bool notifications);
    void run();

    void onMessage(json messageData, std::string type);

    void setEventListener(messageCallBack_ callback);

    MessageInfo getMessage();

    char prefix_;

private:
    MessageInfo MessageInfo_;
    Author author;
    Emoji reaction;
    bool notifications = false;
    messageCallBack_ messageListener;

};

extern MessageEvents messageEventsObject;

bool isCommand(std::string & string);
