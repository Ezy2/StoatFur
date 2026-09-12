#include "./networking.h"

void StoatSession::startHeartbeat() {
    heartbeatTimer_.expires_after(std::chrono::seconds(20));
    heartbeatTimer_.async_wait(beast::bind_front_handler(&StoatSession::onHeartbeat, shared_from_this()));
}

void StoatSession::onHeartbeat(beast::error_code errorCode) {
    if (errorCode) return;

    if (!authenticated) {
        startHeartbeat();
        return;
    }

    json ping = {
        {"type", "Ping"},
        {"data", 0}
    };

    std::string message = ping.dump();

    if (notifications) std::cout << "booping server\n";

    webSocket.async_write( asio::buffer(message),
        beast::bind_front_handler(&StoatSession::onPingWrite, shared_from_this())
    );
}
