OH MAH GAWSH i overwrote my old readme!!! AHH
i had to rewrite it 3: took forever

# intro or what ever

stoatfur was made by me ("Ezy")

its a C++ bot library for stoat.chat

its a little project i am working on by no means is it good but yea

This is not a part of stoat this is unofficial

## some warnings

Most of it was written by me using the docs https://developers.stoat.chat/api-reference

yes this is probably broken i havent made it precompiled for anything

using the CMakeLists.txt in the library/cmake/CMakeLists.txt you can compile by doing cmake build . and cmake --build . in the cmake directory

## requirements

C++ 17 or newer

g++

cmake 3.15 or newer

Boost

Boost.Beast

Boost.System

OpenSSL

nlohmann/json

this was originally tested on archlinux (i use arch btw :3) using zed editor and kde plasma

## contact me

discord: ezy2.0alters
stoat: Ezy2#3695

# License

this project is licensed under the a custom license
see the "LICENSE" file for the full license terms.

# info

## Structs

#### MessageInfo:
  - std::string content;
  - std::string messageID;
  - std::string authorID;
  - std::string authorDisplayName;
  - std::string authorUsername;
  - std::string authorDiscriminator;
  - std::string serverID;
  - std::string channelID;
  - std::string relationship;
  - std::string nonce;
  - std::string joinedDateRaw;
  - std::string botOwner;
  - std::string command;
  - std::string pronouns;
  - std::string edited;
  - std::string embeds; ?? (does nothing)

  - bool isBot;
  - bool isCommand;
  - bool userOnline;

  - std::vector<CommandArgument> arguments;
  - std::vector<std::string> replies;

#### Author:
  - std::string pronouns;
  - std::string botOwner;
  - std::string authorID;
  - std::string authorDisplayName;
  - std::string authorDiscriminator;
  - std::string authorUsername;
  - std::string relationship;
  - std::string joinedDateRaw;

  - bool isBot;
  - bool userOnline;

#### Emoji:
  - std::string ID;
  - std::string creatorID;
  - std::string name;
  - std::string parentID;
  - std::string type;
  - std::string userID;
  - std::string channelID;
  - std::String messageID;

  - bool isAnimated;

#### Event:
  - MessageInfo message;
  - Author author;
  - Emoji emoji;

  - std::string rawData;
  - std::string type;

you can get them by calling them like

```cpp
// for this example varName is a Event

varName.message.content;
varName.author.isBot;
varName.message.arguments[0].argument;
varName.message.arguments[0].isBool();
varName.rawData;

MessageInfo msg = varName.message;

msg.content;

```
### Examples

```cpp
prefix = '!' // using a single char atleast for now dont do std::string just do prefix = 'prefix'

if (event.message.isCommand) { // checks if the message you've recieved (in setEventListener) is a command based on prefix
    if (event.message.command == "hello") { // checks if the command is hello like !hello
        RequestsAPI::sendMessage(event.message.channelID, "Hai there!"); // sends a message
    } else if (event.message.command == "pin") { // if command is pin "!pin" it will pin the current message
        RequestsAPI::pinMessage(event.message.channelID, event.message.messageID); // pins a message
        lastPinID = event.message.messageID; // just to remember the last message for unpinning
    } else if (event.message.command == "unpin") { // if command is unpin "!unpin" it will unpin the last known pin command
        RequestsAPI::unpinMessage(event.message.channelID, lastPinID); // unpins a message
        lastPinID = ""; // resets last pin's id
    }
    std::cout << "argument 1 bool?: " << event.message.arguments[0].isBool() << '\n'; // checks if the first argument is a bool like "!hello true" true is the first argument
}
```

## Functions

#### MessageEventsAPI:
  - setEventListener([](Event varName) {}) // callback lambda

#### RequestsAPI:
  - sendMessage(channelID, message)      // string, string
  - deleteMessage(channelID, messageID)  // string, string
  - pinMessage(channelID, messageID)     // string, string
  - unpinMessage(channelID, messageID)   // string, string

  - addReaction(channelID, messageID, emojiID) // string, string, string

  - getMessage(channelID, messageID, [](Event varName) {}) // string, string, callback lambda
  - getEmoji(emojiID, [](Event varName) {})                // string, string, callback lambda

### Examples

```cpp
MessageEventsAPI::setEventListener([lastPinID](Event event) mutable {
    if (!event.message.isBot && event.message.content == "badword") { // checks if message isnt from a bot and is "badword"
        RequestsAPI::sendMessage(event.message.channelID, "Your message has been removed"); // send message saying it was removed
        RequestsAPI::getMessage(event.message.channelID, event.message.messageID, [event.message](MessageInfo info) { // gets a message again just for testing
            std::cout << info.content << '\n';
            RequestsAPI::deleteMessage(event.message.channelID, event.message.messageID); // deletes inside the get message since get message requires a wait using it after will delete the message before it happens
        });
    }
});
RequestsAPI::getEmoji("01M26H53FA63X6M8F6Y0KE5S3M", [event](Event info) { // gets emoji data
    std::cout << info.rawData << '\n';
});

```

## Events

#### Events:
  - MessageReact
  - Message
  - MessageUnreact
  - MessageDelete
  - MessageUpdate

get them by doing varName.type;

### Examples

```cpp
MessageEventsAPI::setEventListener([lastPinID](Event event) mutable {
    if (event.type == "MessageUpdate") {
        std::cout << "new message content: " << event.message.content << '\n';
    } else if (event.type == "Message") {
        std::cout << event.message.authorID << '\n';
    }
}
```

# Full Example

https://github.com/Ezy2/StoatFur/blob/main/example.cpp

# what i need to add

## interactions

emojis/reactions
creating/getting/deleting a role
set role(s) permissions
edit roles
voice chat stuff?
banning/kicking
embeds
