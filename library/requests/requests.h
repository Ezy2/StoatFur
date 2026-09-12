#pragma once

#include "../includes.h"
#include "../events/events.h"

namespace {
    std::string const apiHost = "api.stoat.chat";
    std::string const apiPort = "443";
}

using messageCallback = std::function<void(Event const &)>;
using json = nlohmann::json;

namespace RequestsAPI {
    void initialize(asio::any_io_executor executor, ssl::context& context, std::string const& token);
    void sendMessage(std::string const & channelID, std::string const & content);
    void deleteMessage(std::string const & channelID, std::string const & messageID);
    void pinMessage(std::string const & channelID, std::string const & messageID);
    void unpinMessage(std::string const & channelID, std::string const & messageID);
    void addReaction(std::string const & channelID, std::string const & messageID, std::string const & emojiID);
    void getMessage(std::string const & channelID, std::string const & messageID, messageCallback callback);
    void getEmoji(std::string const & emojiID, messageCallback callback);
    void getRole(std::string const & serverID, std::string const & roleID, messageCallback callback);
}

namespace asio = boost::asio;
namespace ssl = boost::asio::ssl;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = asio::ip::tcp;

using responseCallback = std::function<void(http::response<http::string_body> const &)>;

struct StoatApi : public std::enable_shared_from_this<StoatApi> {
StoatApi(asio::any_io_executor ex, ssl::context& context, std::string token);

void sendMessage(std::string const channelID, std::string const content);
void deleteMessage(std::string const & channelID, std::string const & messageID);
void pinMessage(std::string const & channelID, std::string const & messageID);
void unpinMessage(std::string const & channelID, std::string const & messageID);
void getMessage(std::string const & channelID, std::string const & messageID, messageCallback callback);
void addReaction(std::string const & channelID, std::string const & messageID, std::string const & emojiID);
void getEmoji(std::string const & emojiID, messageCallback callback);
void getRole(std::string const & serverID, std::string const & roleID, messageCallback callback);

private:
    asio::any_io_executor executor_;
    ssl::context& sslContext_;
    std::string botToken_;
    bool notifications = false;

    void request(http::verb method, std::string target, std::string body = {}, responseCallback callback = {});
};

struct ApiRequest : public std::enable_shared_from_this<ApiRequest> {
ApiRequest(asio::any_io_executor ex, ssl::context& context, std::string token, http::verb method, std::string target, std::string body, responseCallback callback)
 : resolver_(ex), stream_(ex, context), token_(std::move(token)), method_(method), target_(std::move(target)), body_(std::move(body)), callback_(std::move(callback)){}

void run() {
    resolver_.async_resolve(apiHost, apiPort,
        beast::bind_front_handler(&ApiRequest::onResolve, shared_from_this())
    );
}

private:
    tcp::resolver resolver_;
    beast::ssl_stream<beast::tcp_stream> stream_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> request_;
    http::response<http::string_body> response_;
    std::string token_;
    http::verb method_;
    std::string target_;
    std::string body_;
    bool notifications = false;
    responseCallback callback_;

private:
    void onResolve(beast::error_code errorCode, tcp::resolver::results_type results);

    void onConnect(beast::error_code errorCode, tcp::resolver::results_type::endpoint_type);

    void onHandshake(beast::error_code errorCode);

    void onWrite(beast::error_code errorCode, std::size_t);
    void onRead(beast::error_code errorCode, std::size_t);

    void fail(beast::error_code errorCode, const char * what);
};
