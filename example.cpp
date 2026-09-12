#include <fstream>

#include "stoatFur.h"

std::string botToken = "botToken";

int main() {
    std::ifstream file("/home/Ezy/Documents/testing/secrets.txt"); // ur file to bot token can go here it reads the first line
    if (!file.is_open()) {
        std::cerr << "cant open file" << '\n';
        return -1;
    }
    std::getline(file, botToken);

    prefix = '!';
    std::string lastPinID = "";

    MessageEventsAPI::setEventListener([lastPinID](Event event) mutable {
        if (event.type == "MessageUpdate") {
            std::cout << "new message content: " << event.message.content << '\n';
        }
        if (event.type == "Message") {
            if (!event.author.isBot && event.message.content == "badword") {
                RequestsAPI::sendMessage(event.message.channelID, "Your message has been removed");
                RequestsAPI::getMessage(event.message.channelID, event.message.messageID, [event](Event info) {
                    std::cout << "removed word: " << event.message.content << '\n';
                    RequestsAPI::deleteMessage(event.message.channelID, event.message.messageID);
                });
            }

            if (event.message.isCommand) {
                if (event.message.command == "hello") {
                    if (event.message.arguments.size() > 0 && event.message.arguments[0].argument == "bot") {
                        RequestsAPI::sendMessage(event.message.channelID, "Hai " + event.author.authorDisplayName + "!");
                    } else {
                        RequestsAPI::sendMessage(event.message.channelID, "Hai there!");
                    }
                } else if (event.message.command == "pin") {
                    RequestsAPI::pinMessage(event.message.channelID, event.message.messageID);
                    lastPinID = event.message.messageID;
                } else if (event.message.command == "unpin") {
                    RequestsAPI::unpinMessage(event.message.channelID, lastPinID);
                    lastPinID = "";
                } else if (event.message.command == "getemoji" && event.message.arguments.size() > 0) {
                    std::cout << event.message.arguments[0].argument << '\n';
                    RequestsAPI::sendMessage(event.message.channelID, event.message.arguments[0].argument);
                    RequestsAPI::getEmoji("01M26H53FA63X6M8F6Y0KE5S3M", [event](Event info) {
                        std::cout << info.emoji.name << '\n';
                        std::cout << info.emoji.ID << '\n';
                    });
                } else if (event.message.command == "MessageReact") {
                    RequestsAPI::addReaction(event.message.channelID, event.message.messageID, "01M26H53FA63X6M8F6Y0KE5S3M");
                }
                // if (event.message.arguments.size() > 0) { // just kept annoying me
                //     std::cout << "argument 1 bool?: " << event.message.arguments[0].isBool() << '\n';
                // }
            }
        } else if (event.type == "MessageReact") {
            std::cout << "Who reacted: " << event.emoji.userID << '\n';
        }

        for (auto const & reply : event.message.replies) {
            std::cout << reply << '\n';
        }
    });

    runClient(botToken);
}
