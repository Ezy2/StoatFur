#include "./requests.h"
#include "../parsing/parse.h"

StoatApi::StoatApi(asio::any_io_executor ex, ssl::context& context, std::string token) : executor_(ex), sslContext_(context), botToken_(std::move(token)) {}

namespace {
    std::unique_ptr<StoatApi> requestsObject;
}

namespace RequestsAPI {
    void initialize(asio::any_io_executor executor, ssl::context& context, std::string const & token) {
        requestsObject = std::make_unique<StoatApi>(executor, context, token);
    }

    bool isInitialized() {
        if (!requestsObject) {
            std::cerr << "RequestsAPI has not been initialized" << '\n';
            return false;
        }
        return true;
    }

    void sendMessage(std::string const & channelID, std::string const & content, const std::vector<Embed> & embeds) {
        if (!isInitialized()) return;
        requestsObject->sendMessage(channelID, content, embeds);
    }

    void deleteMessage(std::string const & channelID, std::string const & messageID) {
        if (!isInitialized()) return;
        requestsObject->deleteMessage(channelID, messageID);
    }

    void pinMessage(std::string const & channelID, std::string const & messageID) {
        if (!isInitialized()) return;
        requestsObject->pinMessage(channelID, messageID);
    }

    void unpinMessage(std::string const & channelID, std::string const & messageID) {
        if (!isInitialized()) return;
        requestsObject->unpinMessage(channelID, messageID);
    }

    void getMessage(std::string const & channelID, std::string const & messageID, messageCallback callback) {
        if (!isInitialized()) return;
        requestsObject->getMessage(channelID, messageID, std::move(callback));
    }

    void getEmoji(std::string const & emojiID, messageCallback callback) {
        if (!isInitialized()) return;
        requestsObject->getEmoji(emojiID, std::move(callback));
    }

    void addReaction(std::string const & channelID, std::string const & messageID, std::string const & emojiID) {
        if (!isInitialized()) return;
        requestsObject->addReaction(channelID, messageID, emojiID);
    }

    void getRole(std::string const & serverID, std::string const & roleID, messageCallback callback) {
        if (!isInitialized()) return;
        requestsObject->getRole(serverID, roleID, std::move(callback));
    }

    void addRole(const std::string serverID, const std::string memberID, std::string const roleID) {
        if (!isInitialized()) return;
        requestsObject->addRole(serverID, memberID, roleID);
    }

    void removeRole(const std::string serverID, const std::string memberID, std::string const roleID) {
        if (!isInitialized()) return;
        requestsObject->removeRole(serverID, memberID, roleID);
    }

    void getUser(std::string const & serverID, std::string const & userID, messageCallback callback) {
        if (!isInitialized()) return;
        requestsObject->getUser(serverID, userID, std::move(callback));
    }
}


void StoatApi::sendMessage(const std::string channelID, const std::string content, const std::vector<Embed> embeds) {
    std::vector<json> allEmbeds = {};

    for (const auto& embed : embeds) {
        json embedJson;

        embedJson["icon_url"] = embed.iconURL.empty() ? json(nullptr) : json(embed.iconURL);
        embedJson["url"] = embed.url.empty() ? json(nullptr) : json(embed.url);
        embedJson["title"] = embed.title.empty() ? json(nullptr) : json(embed.title);
        embedJson["description"] = embed.description.empty() ? json(nullptr) : json(embed.description);
        embedJson["media"] = embed.media.empty() ? json(nullptr) : json(embed.media);
        embedJson["colour"] = embed.colour.empty() ? json(nullptr) : json(embed.colour);

        allEmbeds.push_back(embedJson);
    }

    json body = {
        {"content", content},
        {"embeds", allEmbeds}
    };

    request(http::verb::post, "/channels/" + channelID + "/messages", body.dump());
}

void StoatApi::addRole(const std::string serverID, const std::string memberID, const std::string roleID) {
    getUser(serverID, memberID, [this, serverID, memberID, roleID](Event info) {
        std::vector<std::string> roles;
        roles.push_back(roleID);
        for (const auto& role : info.author.roles) {
            if (role == roleID) continue;
            roles.push_back(role);
        }
        json body = {
            {"roles", roles}
        };
        request(http::verb::patch, "/servers/" + serverID + "/members/" + memberID, body.dump());
    });
}

void StoatApi::removeRole(const std::string serverID, const std::string memberID, const std::string roleID) {
    getUser(serverID, memberID, [this, serverID, memberID, roleID](Event info) {
        std::vector<std::string> roles;
        for (const auto& role : info.author.roles) {
            if (role != roleID) {
                roles.push_back(role);
            }
        }
        json body = {
            {"roles", roles}
        };
        request(http::verb::patch, "/servers/" + serverID + "/members/" + memberID, body.dump());
    });
}

void StoatApi::deleteMessage(std::string const & channelID, std::string const & messageID) {
    request(http::verb::delete_, "/channels/" + channelID + "/messages/" + messageID);
}

void StoatApi::pinMessage(std::string const & channelID, std::string const & messageID) {
    request(http::verb::post, "/channels/" + channelID + "/messages/" + messageID + "/pin");
}

void StoatApi::unpinMessage(std::string const & channelID, std::string const & messageID) {
    request(http::verb::delete_, "/channels/" + channelID + "/messages/" + messageID + "/pin");
}

void StoatApi::addReaction(std::string const & channelID, std::string const & messageID, std::string const & emojiID) {
    request(http::verb::put, "/channels/" + channelID + "/messages/" + messageID + "/reactions/" + emojiID);
}

void StoatApi::getMessage(std::string const & channelID, std::string const & messageID, messageCallback callback) {
    request(http::verb::get, "/channels/" + channelID + "/messages/" + messageID, {}, [callback = std::move(callback)](auto const & response) mutable {
        Event event;
        parseAllData(response.body(), event);
        callback(event);
    });
}

void StoatApi::getEmoji(std::string const & emojiID, messageCallback callback) {
    request(http::verb::get, "/custom/emoji/" + emojiID, {}, [callback = std::move(callback)](responseCallback::argument_type const & response) mutable {
        Event event;
        parseAllData(response.body(), event);
        callback(event);
    });
}

void StoatApi::getRole(std::string const & serverID, std::string const & roleID, messageCallback callback) {
    request(http::verb::get, "/servers/" + serverID + "/roles/" + roleID, {}, [callback = std::move(callback)](responseCallback::argument_type const & response) mutable {
        Event event;
        parseAllData(response.body(), event);
        callback(event);
    });
}

void StoatApi::getUser(std::string const & serverID, std::string const & userID, messageCallback callback) {
    request(http::verb::get, "/servers/" + serverID + "/members/" + userID, {}, [callback = std::move(callback)](responseCallback::argument_type const & response) mutable {
        Event event;
        parseAllData(response.body(), event);
        callback(event);
    });
}

void StoatApi::request(http::verb method, std::string target, std::string body, responseCallback callback) {
    auto req = std::make_shared<ApiRequest>(executor_, sslContext_, botToken_, method, std::move(target), std::move(body), std::move(callback));
    req->run();
}

void parseAllData(std::string data, Event & event) {
    try {
        json body = json::parse(data);
        event.rawData = json::parse(data).dump(4);
        parseMessageData(json::parse(data), event.message, "getEmoji");
        parseAuthorData(json::parse(data), event.author, "getEmoji");
        parseEmojiData(json::parse(data), event.emoji);
        parseRoleData(json::parse(data), event.role);
    } catch (std::exception const error) {
        // do nuthin :3
    }
}
