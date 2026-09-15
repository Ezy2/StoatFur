// dis is what ive been using for testing stuffz

#include <fstream>

#include "stoatFur.h"

std::string botToken = "botToken";

void sendEmbeds(Event & event) {
    std::vector<Embed> embeds;

    Embed embed;
    Embed embed2;

    embed.colour = "pink";
    embed.title = "test!";

    embed2.colour = "red";
    embed2.title = "test@";

    embeds.push_back(embed);
    embeds.push_back(embed2);

    RequestsAPI::sendMessage(event.message.channelID, "Testing Embeds", embeds);
}

int main() {
    std::ifstream file("/home/Ezy/Documents/testing/secrets.txt"); // ur file to bot token can go here it reads the first line
    if (!file.is_open()) {
        std::cerr << "cant open file" << '\n';
        return -1;
    }
    std::getline(file, botToken);

    prefix = '!';
    std::string lastPinID = "";
    std::string emojiID = "01M26H53FA63X6M8F6Y0KE5S3M";
    std::string roleID = "01M2C4A5N3BZY1XCC9GRFNN9RZ";

    std::string redRole = "01M2GWQ6111RXBA7T6J2NHVZ06";
    std::string pinkRole = "01M2GWQPE6S5HZQ11G8CA3DNA1";
    std::string roleMessage = "01M2GWPJ625Z31QGFNPRP30DH9";
    std::string serverID = "01M1ZRQGRWNR925WD85EJ6X7FK";

    MessageEventsAPI::setEventListener([&](Event event) mutable {
        if (event.type == "MessageUpdate") {
            std::cout << "new message content: " << event.message.content << '\n';
        }

        if (event.type == "Message") {
            for (auto const & role : event.author.roles) {
                // std::cout << "Role ID: " << role << '\n';
                RequestsAPI::getRole(event.message.serverID, role, [](Event info) {
                    // std::cout << info.role.seperate << '\n';
                });
            }

            // std::cout << event.author.nickname << "B" << '\n';

            RequestsAPI::getUser(event.message.serverID, event.message.authorID, [](Event info) {
                for (auto const & role : info.author.roles) {
                    // std::cout << role << '\n';
                }
            });

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
                    RequestsAPI::getEmoji(emojiID, [event](Event info) {
                        std::cout << info.emoji.name << '\n';
                        std::cout << info.emoji.ID << '\n';
                    });
                } else if (event.message.command == "addreaction") {
                    RequestsAPI::addReaction(event.message.channelID, event.message.messageID, emojiID);
                } else if (event.message.command == "giverole") {
                    RequestsAPI::addRole(event.message.serverID, event.message.authorID, roleID);
                } else if (event.message.command == "removerole") {
                    RequestsAPI::removeRole(event.message.serverID, event.message.authorID, roleID);
                } else if (event.message.command == "embedtests") {
                    sendEmbeds(event);
                }
                // if (event.message.arguments.size() > 0) { // just kept annoying me
                //     std::cout << "argument 1 bool?: " << event.message.arguments[0].isBool() << '\n';
                // }
            }
        } else if (event.type == "MessageReact") { // no clue how u get the serverid from this event
            if (event.message.messageID == roleMessage) {
                if (event.emoji.ID == "❤") {
                    RequestsAPI::addRole(serverID, event.emoji.userID, redRole);
                } else if (event.emoji.ID == "🩷") {
                    RequestsAPI::addRole(serverID, event.emoji.userID, pinkRole);
                }
            }
        } else if (event.type == "MessageUnreact") {
            if (event.message.messageID == roleMessage) {
                if (event.emoji.ID == "❤") {
                    RequestsAPI::removeRole(serverID, event.emoji.userID, redRole);
                } else if (event.emoji.ID == "🩷") {
                    RequestsAPI::removeRole(serverID, event.emoji.userID, pinkRole);
                }
            }
        }

        for (auto const & reply : event.message.replies) {
            std::cout << reply << '\n';
        }
    });

    runClient(botToken);
}
