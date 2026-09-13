#include "parse.h"
#include "../events/events.h"
#include "../../stoatFur.h"

void parseMessageData(json messageData, MessageInfo & info, std::string type) {
    info.content = messageData.value("content", "");
    info.nonce = messageData.value("nonce", "");
    info.isCommand = !info.content.empty() && info.content[0] == prefix;
    info.channelID = messageData.value("channel", "");

    if (info.messageID == "") info.messageID = messageData.value("id", "");

    if (messageData.contains("data")) {
        info.edited = messageData["data"].value("edited", "");
        if (info.content == "") info.content = messageData["data"].value("content", "");
    }

    if (messageData.contains("member")) {
        info.joinedDateRaw = messageData["member"].value("joined_at", "");
        if (messageData["member"].contains("_id")) {
            info.serverID = messageData["member"]["_id"].value("server", "");
        }
    }

    info.isBot = false;

    if (messageData.contains("user") && (type != "ChannelStartTyping" && type != "ChannelStopTyping")) { // mahbee decapritated? its all under author now
        info.authorDiscriminator = messageData["user"].value("discriminator", "");
        info.authorDisplayName = messageData["user"].value("display_name", "");
        info.relationship = messageData["user"].value("relationship", "");
        info.authorUsername = messageData["user"].value("username", "");
        info.userOnline = messageData["user"].value("online", false);
        info.pronouns = messageData["user"].value("pronouns", "");
        info.authorID = messageData["user"].value("_id", "");
    }
    if (messageData["user"].contains("bot")) {
        info.isBot = true;
        info.botOwner = messageData["user"]["bot"].value("owner", "");
    }
    if (messageData["_id"].is_string()) {
        info.messageID = messageData["_id"].get<std::string>();
    }

    info.arguments = {};
    info.replies = {};

    if (info.isCommand) {
        getArguments(info.arguments, info.content);
        if (!info.arguments.empty()) {
            info.arguments.erase(info.arguments.begin());
            info.command = info.arguments[0].argument.substr(1);
        }
    }

    info.replies = messageData.value("replies", json::array());
}

void parseAuthorData(json authorData, Author & info, std::string type) {
    info.isBot = false;

    if (authorData.contains("_id") && authorData["_id"].is_object()) {
        info.authorID = authorData["_id"].value("user", "");
        info.serverID = authorData["_id"].value("server", "");
    }

    info.roles = authorData.value("roles", json::array());

    if (authorData.contains("member")) {
        info.joinedDateRaw = authorData["member"].value("joined_at", "");
        info.nickname = authorData["member"].value("nickname", "");
        if (info.roles.size() <= 0) info.roles = authorData["member"].value("roles", json::array());
    } else {
        info.nickname = authorData.value("nickname", "");
    }

    if (authorData.contains("user") && authorData["user"].is_object() && type != "ChannelStartTyping" && type != "ChannelStopTyping") {
        info.authorDiscriminator = authorData["user"].value("discriminator", "");
        info.authorDisplayName = authorData["user"].value("display_name", "");
        info.relationship = authorData["user"].value("relationship", "");
        info.authorUsername = authorData["user"].value("username", "");
        info.userOnline = authorData["user"].value("online", false);
        info.pronouns = authorData["user"].value("pronouns", "");

        if (authorData["user"].contains("_id") && authorData["user"]["_id"].is_string()) {
            info.authorID = authorData["user"]["_id"].get<std::string>();
        }

        if (authorData["user"].contains("bot") && authorData["user"]["bot"].is_object()) {
            info.isBot = true;
            info.botOwner = authorData["user"]["bot"].value("owner", "");
        }
    }

    if (authorData.contains("avatar") && authorData["avatar"].is_object()) {
        info.iconID = authorData["avatar"].value("_id", "");
        info.tag = authorData["avatar"].value("tag", "");
        info.filename = authorData["avatar"].value("filename", "");
        info.contentType = authorData["avatar"].value("content_type", "");
        info.size = authorData["avatar"].value("size", 0);
        info.deleted = authorData["avatar"].value("deleted", false);
        info.reported = authorData["avatar"].value("reported", false);
        info.messageID = authorData["avatar"].value("message_id", "");
        info.userID = authorData["avatar"].value("user_id", "");
        info.serverID = authorData["avatar"].value("server_id", "");
        info.objectID = authorData["avatar"].value("object_id", "");
    }

    if (info.pronouns.empty()) {
        info.pronouns = authorData.value("pronouns", "");
    }
    if (info.joinedDateRaw.empty()) {
        info.joinedDateRaw = authorData.value("joined_at", "");
    }
}

void parseReactedData(json reactedData, Emoji & info, std::string type) {
    info.channelID = reactedData.value("channel_id", "");
    info.ID = reactedData.value("emoji_id", "");
    info.messageID = reactedData.value("id", "");
    info.type = reactedData.value("type", "");
    info.userID = reactedData.value("user_id", "");
}

// these are get requests ones hence no "type"

void parseRoleData(json roleData, Role & info) {
    info.colour = roleData.value("colour", "");
    info.seperate = roleData.value("hoist", false);
    info.name = roleData.value("name", "");
    info.rank = roleData.value("rank", 0);

    if (roleData.contains("permissions")) {
        info.allowed = roleData["permissions"].value("a", 0);
        info.disallowed = roleData["permissions"].value("d", 0);
    }

    if (roleData.contains("icon")) {
        info.iconID = roleData["icon"].value("_id", "");
        info.tag = roleData["icon"].value("tag", "");
        info.filename = roleData["icon"].value("filename", "");
        info.contentType = roleData["icon"].value("content_type", "");
        info.size = roleData["icon"].value("size", 0);
        info.deleted = roleData["icon"].value("deleted", false);
        info.reported = roleData["icon"].value("reported", false);
        info.messageID = roleData["icon"].value("message_id", "");
        info.userID = roleData["icon"].value("user_id", "");
        info.serverID = roleData["icon"].value("server_id", "");
        info.objectID = roleData["icon"].value("object_id", "");
    }
    if (roleData["_id"].is_string()) {
        info.ID = roleData["_id"].get<std::string>();
    }
}

void parseEmojiData(json emojiData, Emoji & info) {
    info.isAnimated = emojiData.value("animated", false);
    info.creatorID = emojiData.value("creator_id", "");
    info.name = emojiData.value("name", "");

    if (emojiData.contains("parent")) {
        info.parentID = emojiData["parent"].value("id", "");
        info.type = emojiData["parent"].value("type", "");
    }
    if (emojiData["_id"].is_string()) {
        info.ID = emojiData["_id"].get<std::string>();
    }
}
