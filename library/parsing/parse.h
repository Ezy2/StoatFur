#pragma once

#include "../includes.h"

struct CommandArgument {
    std::string argument;
    operator std::string() const {return argument;}

    bool isBool() const {
        return (argument == "true" || argument == "false" || argument == "1" || argument == "0");
    }
};

struct MessageInfo {
    std::string content = "";
    std::string messageID = "";
    std::string authorID = "";
    std::string authorDisplayName = "";
    std::string authorUsername = "";
    std::string authorDiscriminator = "";
    std::string serverID = "";
    std::string channelID = "";
    std::string relationship = "";
    std::string nonce = ""; // decapritated from what i heard
    std::string joinedDateRaw = "";
    std::string botOwner = "";
    std::string command = "This message was not a command";
    std::string pronouns = "";
    std::string edited = "";
    std::string embeds = "";

    bool isBot = false;
    bool isCommand = false;
    bool userOnline = false;

    std::vector<CommandArgument> arguments;
    std::vector<std::string> replies;

private:
    bool response = false;
};

struct Author {
    std::string pronouns = "";
    std::string botOwner = "";
    std::string authorID = "";
    std::string authorDisplayName = "";
    std::string authorDiscriminator = "";
    std::string authorUsername = "";
    std::string relationship = "";
    std::string joinedDateRaw = "";

    bool isBot = false;
    bool userOnline = false;

    std::vector<std::string> roles;
};

struct Emoji {
    std::string ID = "";
    std::string creatorID = "";
    std::string name = "";
    std::string parentID = "";
    std::string type = "";
    std::string userID = "";
    std::string channelID = "";
    std::string messageID = "";

    bool isAnimated = false;
};

struct Role {
    std::string ID = "";
    std::string name = "";
    std::string colour = "";
    std::string iconID = "";
    std::string tag = "";
    std::string filename = "";
    std::string contentType = "";
    std::string messageID = "";
    std::string userID = "";
    std::string serverID = "";
    std::string objectID = "";

    int size = 0;
    std::int64_t rank = 0;
    std::int64_t allowed = 0;
    std::int64_t disallowed = 0;

    bool seperate = false;
    bool deleted = false;
    bool reported = false;
};

struct Event {
    MessageInfo message;
    Author author;
    Emoji emoji;
    Role role;

    std::string rawData = "";
    std::string type = "";
};

void parseMessageData(json messageData, MessageInfo & info, std::string type);
void parseAuthorData(json authorData, Author & info, std::string type);
void parseReactedData(json reactedData, Emoji & info, std::string type);
void parseEmojiData(json emojiData, Emoji & info);
void parseRoleData(json roleData, Role & info);
void getArguments(std::vector<CommandArgument> & vector, std::string string);
