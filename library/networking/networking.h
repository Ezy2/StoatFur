#pragma once

#include "../includes.h"
#include "../requests/requests.h"

namespace {
    const std::string host = "events.stoat.chat";
    const std::string port = "443";
    const std::string target = "/?version=1&format=json";
}

namespace asio = boost::asio;
namespace ssl = boost::asio::ssl;
namespace beast = boost::beast;
namespace websocket = beast::websocket;

using tcp = asio::ip::tcp;
using json = nlohmann::json;

struct StoatApi;

struct StoatSession : public std::enable_shared_from_this<StoatSession> {
    StoatSession(asio::any_io_executor ex, ssl::context& context, std::string token);
    void run();

private:
    tcp::resolver resolver;
    websocket::stream<beast::ssl_stream<beast::tcp_stream>> webSocket;
    beast::flat_buffer buffer;
    asio::steady_timer heartbeatTimer_;
    std::string botToken;
    bool authenticated = false;
    std::shared_ptr<StoatApi> api;
    bool notifications = false;

private:
    void onResolve(beast::error_code errorCode, tcp::resolver::results_type results);

    void onConnect(beast::error_code errorCode, tcp::resolver::results_type::endpoint_type ep);

    void onHandshake(beast::error_code errorCode);
    void onWebsocket(beast::error_code errorCode);

    void onAuthWrite(beast::error_code errorCode, std::size_t bytes_transferred);

    void doRead();

    void onRead(beast::error_code errorCode, std::size_t bytes_transferred);

    void handleEvent(json const & data);

    void startHeartbeat();
    void onHeartbeat(beast::error_code errorCode);

    void onPingWrite(beast::error_code errorCode, std::size_t bytes_transferred);

    void fail(beast::error_code errorCode, char const * what);
};

int runClient(std::string& botToken);
