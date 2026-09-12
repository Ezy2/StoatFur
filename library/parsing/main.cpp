#include "parse.h"
#include "../events/events.h"
#include "../../stoatFur.h"

void parseMessageData(json messageData, MessageInfo & info, std::string type) {
    info.content = messageData.value("content", "");
    info.messageID = messageData.value("_id", "");
    info.nonce = messageData.value("nonce", "");
    info.isCommand = (info.content[0] == prefix);
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

    info.arguments = {};
    info.replies = {};

    if (info.isCommand) {
        getArguments(info.arguments, info.content);
        info.command = info.arguments[0].argument.substr(1);
        info.arguments.erase(info.arguments.begin());
    }

    info.replies = messageData.value("replies", json::array());
}

void parseAuthorData(json authorData, Author & info, std::string type) {
    if (authorData.contains("member")) {
        info.joinedDateRaw = authorData["member"].value("joined_at", "");
        info.roles = authorData["member"].value("roles", json::array());
    }

    if (authorData.contains("user") && (type != "ChannelStartTyping" && type != "ChannelStopTyping")) {
        info.authorDiscriminator = authorData["user"].value("discriminator", "");
        info.authorDisplayName = authorData["user"].value("display_name", "");
        info.relationship = authorData["user"].value("relationship", "");
        info.authorUsername = authorData["user"].value("username", "");
        info.userOnline = authorData["user"].value("online", false);
        info.pronouns = authorData["user"].value("pronouns", "");
        info.authorID = authorData["user"].value("_id", "");
    }

    info.isBot = false;

    if (authorData["user"].contains("bot")) {
        info.isBot = true;
        info.botOwner = authorData["user"]["bot"].value("owner", "");
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
    info.ID = roleData.value("_id", "");
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
}

void parseEmojiData(json emojiData, Emoji & info) {
    info.ID = emojiData.value("_id", "");
    info.isAnimated = emojiData.value("animated", false);
    info.creatorID = emojiData.value("creator_id", "");
    info.name = emojiData.value("name", "");

    if (emojiData.contains("parent")) {
        info.parentID = emojiData["parent"].value("id", "");
        info.type = emojiData["parent"].value("type", "");
    }
}
