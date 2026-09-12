#include "./requests.h"
#include "../events/events.h"

void ApiRequest::onResolve(beast::error_code errorCode, tcp::resolver::results_type results) {
    if (errorCode) {
        fail(errorCode, "resolve error");
        return;
    }

    if (!SSL_set_tlsext_host_name(stream_.native_handle(), apiHost.c_str())) {
        beast::error_code sslerrorCode{static_cast<int>(::ERR_get_error()), asio::error::get_ssl_category()};
        fail(sslerrorCode, "ssl error");
        return;
    }

    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
    beast::get_lowest_layer(stream_).async_connect(results,
        beast::bind_front_handler(&ApiRequest::onConnect, shared_from_this())
    );
}

void ApiRequest::onConnect(beast::error_code errorCode, tcp::resolver::results_type::endpoint_type) {
    if (errorCode) {
        fail(errorCode, "connection error");
        return;
    }

    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    stream_.async_handshake(ssl::stream_base::client,
        beast::bind_front_handler(&ApiRequest::onHandshake, shared_from_this())
    );
}

void ApiRequest::onHandshake(beast::error_code errorCode) {
    if (errorCode) {
        fail(errorCode, "handshake failed");
        return;
    }

    request_.version(11);
    request_.method(method_);
    request_.target(target_);

    request_.set(http::field::host, apiHost);
    request_.set(http::field::user_agent, "Stoat-CPP-botLib");
    request_.set("X-Bot-Token", token_);

    if (!body_.empty()) {
        request_.set(http::field::content_type, "application/json");
        request_.body() = body_;
        request_.prepare_payload();
    }

    http::async_write(stream_, request_,
        beast::bind_front_handler(&ApiRequest::onWrite, shared_from_this())
    );
}

void ApiRequest::onWrite(beast::error_code errorCode, std::size_t) {
    if (errorCode) {
        fail(errorCode, "failed to write");
        return;
    }

    http::async_read(stream_, buffer_, response_,
        beast::bind_front_handler(&ApiRequest::onRead, shared_from_this())
    );
}

void ApiRequest::onRead(beast::error_code errorCode, std::size_t) {
    json responseJson;

    if (errorCode) {
        fail(errorCode, "failed to read");
        return;
    }

    if (callback_) {
        callback_(response_);
    }

    if (notifications) {
        std::cout << "response: " << response_.result_int() << " " << response_.reason() << '\n';
        if (!response_.body().empty()) std::cout << response_.body() << '\n';
    }

    beast::error_code shutdownerrorCode;
    stream_.shutdown(shutdownerrorCode);

    if (shutdownerrorCode && shutdownerrorCode != asio::error::eof && shutdownerrorCode != ssl::error::stream_truncated)
        fail(shutdownerrorCode, "shutdown");
}

void ApiRequest::fail(beast::error_code errorCode, const char * what) {
    std::cerr << what << ": " << errorCode.message() + " [REQUESTS]" << '\n';
}
