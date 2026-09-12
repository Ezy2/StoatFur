#include "./networking.h"
#include "../../stoatFur.h"

StoatSession::StoatSession(asio::any_io_executor ex, ssl::context & context, std::string token) : resolver(ex), webSocket(ex, context), heartbeatTimer_(ex), botToken(std::move(token)) {
    api = std::make_shared<StoatApi>(ex, context, botToken);
}

void StoatSession::run() {
    resolver.async_resolve(host, port,
        beast::bind_front_handler(&StoatSession::onResolve, shared_from_this())
    );
}

void StoatSession::onResolve(beast::error_code errorCode, tcp::resolver::results_type results) {
    if (errorCode) return fail(errorCode, "resolve");

    if (notifications) std::cout << "DNS resolved" << '\n';

    beast::get_lowest_layer(webSocket).expires_after(std::chrono::seconds(30));

    beast::get_lowest_layer(webSocket).async_connect(results,
        beast::bind_front_handler(&StoatSession::onConnect, shared_from_this())
    );
}

void StoatSession::onConnect(beast::error_code errorCode, tcp::resolver::results_type::endpoint_type _) {
    boost::ignore_unused(_);

    if (errorCode) return fail(errorCode, "connect");

    if (!SSL_set_tlsext_host_name(webSocket.next_layer().native_handle(), host.c_str())) {
        beast::error_code ssl_ec{static_cast<int>(::ERR_get_error()), asio::error::get_ssl_category()};
        return fail(ssl_ec, "SNI");
    }

    beast::get_lowest_layer(webSocket).expires_never();

    webSocket.next_layer().async_handshake(
        ssl::stream_base::client,
        beast::bind_front_handler(&StoatSession::onHandshake, shared_from_this())
    );
}

void StoatSession::onHandshake(beast::error_code errorCode) {
    if (errorCode) return fail(errorCode, "ssl_handshake");

    webSocket.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));
    webSocket.set_option(websocket::stream_base::decorator([](websocket::request_type& req) {
        req.set(beast::http::field::user_agent, "Stoat-CPP-botLib");
    }));

    webSocket.async_handshake(host, target, beast::bind_front_handler(&StoatSession::onWebsocket, shared_from_this()));
}

void StoatSession::onWebsocket(beast::error_code errorCode) {
    if (errorCode) return fail(errorCode, "websocket_handshake");

    json payload = {
        {"type", "Authenticate"},
        {"token", botToken}
    };

    std::string message = payload.dump();

    webSocket.text(true);
    webSocket.async_write(asio::buffer(message), beast::bind_front_handler(&StoatSession::onAuthWrite, shared_from_this()));
}

void StoatSession::onAuthWrite(beast::error_code errorCode, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (errorCode) return fail(errorCode, "authenticate_write");

    if (notifications) std::cout << "authentication sent\n";

    doRead();
}

void StoatSession::doRead() {
    webSocket.async_read(buffer, beast::bind_front_handler(&StoatSession::onRead, shared_from_this()));
}

void StoatSession::onRead(beast::error_code errorCode, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (errorCode) {
        if (errorCode == websocket::error::closed) std::cerr << "webSocket closed" << '\n';
        else fail(errorCode, "read");
        return;
    }

    try {
        std::string message = beast::buffers_to_string(buffer.data());
        auto data = json::parse(message);

        handleEvent(data);
    }
    catch (std::exception const & error) {
        std::cerr << "json error: " << error.what() << '\n';
    }

    buffer.consume(buffer.size());

    doRead();
}

void StoatSession::onPingWrite(beast::error_code errorCode, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (errorCode) return fail(errorCode, "ping_write");
    startHeartbeat();
}

void StoatSession::fail(beast::error_code errorCode, char const * what) {
    std::cerr << what << ": " << errorCode.message() + " [EVENTS]" << '\n';
}
