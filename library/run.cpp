#include "./includes.h"
#include "./networking/networking.h"
#include "./requests/requests.h"
#include "./events/events.h"
#include "../stoatFur.h"

char prefix = '\n';

bool messageNotifications = false;

int runClient(std::string& botToken) {
    if (botToken.empty()) {
        std::cerr << "please enter a bot token" << '\n';
        return 1;
    }

    asio::io_context ioc;
    ssl::context context{ssl::context::tls_client};

    try {
        context.set_default_verify_paths();
    } catch (std::exception const & e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return 1;
    }

    context.set_verify_mode(ssl::verify_peer);

    RequestsAPI::initialize(asio::make_strand(ioc), context, botToken);
    auto session = std::make_shared<StoatSession>(asio::make_strand(ioc), context, botToken);
    auto messages = std::make_shared<MessageEvents>(prefix, messageNotifications);

    session->run();
    messages->run();
    ioc.run();

    return 0;
}
