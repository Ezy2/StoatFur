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

    void sendMessage(std::string const & channelID, std::string const & content) {
        if (!isInitialized()) return;
        requestsObject->sendMessage(channelID, content);
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
}

void StoatApi::sendMessage(const std::string channelID, const std::string content) {
    json body = {
        {"content", content}
    };

    request(http::verb::post, "/channels/" + channelID + "/messages", body.dump());
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
        event.rawData = json::parse(response.body()).dump(4);
        parseMessageData(json::parse(response.body()), event.message);
        parseAuthorData(json::parse(response.body()), event.author);
        callback(event);
    });
}

void StoatApi::getEmoji(std::string const & emojiID, messageCallback callback) {
    request(http::verb::get, "/custom/emoji/" + emojiID, {}, [callback = std::move(callback)](responseCallback::argument_type const & response) mutable {
        Event event;
        event.rawData = json::parse(response.body()).dump(4);
        parseMessageData(json::parse(response.body()), event.message);
        parseAuthorData(json::parse(response.body()), event.author);
        parseEmojiData(json::parse(response.body()), event.emoji);
        callback(event);
    });
}

void StoatApi::request(http::verb method, std::string target, std::string body, responseCallback callback) {
    auto req = std::make_shared<ApiRequest>(executor_, sslContext_, botToken_, method, std::move(target), std::move(body), std::move(callback));
    req->run();
}
