OH MAH GAWSH i overwrote my old readme!!! AHH
i had to rewrite it 3: took forever (currently past where i was but i do hope it doestn get ruined again)

# intro or what ever

stoatfur was made by me ("Ezy")

its a C++ bot library for stoat.chat even though several already exists

its a little project i am working on by no means is it good but yea

This is not a part of stoat this is unofficial

## why you should choose it?

i dont think u should its very unprofessional and its my first time making something like this

## some warnings

was made using the docs https://developers.stoat.chat/api-reference

shoutout to whomever made them so clear to read :3

yes this is probably broken i havent made it precompiled for anything

using the CMakeLists.txt in the library/cmake/CMakeLists.txt you can compile by doing cmake build . and cmake --build . in the cmake directory

## requirements

C++ 17 or newer

a cmake compatible cpp compiler like g++

cmake 3.15 or newer

Boost

Boost.Beast

Boost.System

OpenSSL

nlohmann/json

this was originally tested on archlinux (i use arch btw :3) using zed editor and kde plasma using

## contact me

discord: ezy2.0alters
stoat: Ezy2#3695

# License

this project is licensed under the a custom license
see the "LICENSE" file for the full license terms.

# info

## Structs

ill eventually have to rework these MAYBE if i feel like making it better cause you dont need authorID in messageInfo if its already in author but its fineee choices am i right?

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
  - std::string embeds; ?? (does nothing yet) (side note: will probably be added to the Embed struct)

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
  - std::string ID = "";
  - std::string name = "";
  - std::string colour = "";
  - std::string iconID = "";
  - std::string tag = "";
  - std::string filename = "";
  - std::string contentType = "";
  - std::string messageID = "";
  - std::string userID = "";
  - std::string serverID = "";
  - std::string objectID = "";
  - std::string nickname = "";

  - int size = 0;
  - std::int64_t rank = 0;
  - std::int64_t allowed = 0;
  - std::int64_t disallowed = 0;

  - bool seperate = false;
  - bool deleted = false;
  - bool reported = false;
  - bool userOnline = false;
  - bool isBot = false;

  - std::vector<std::string> roles;

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

#### Role:
  - std::string ID;
  - std::string name;
  - std::string colour;
  - std::string iconID;
  - std::string tag;
  - std::string filename;
  - std::string contentType;
  - std::string messageID;
  - std::string userID;
  - std::string serverID;
  - std::string objectID;

  - int size;
  - std::int64_t rank;
  - std::int64_t allowed;
  - std::int64_t disallowed;

  - bool seperate;
  - bool deleted;
  - bool reported;

#### Event:
  - MessageInfo message;
  - Author author;
  - Emoji emoji;

  - std::string rawData;
  - std::string type;

#### Embed: (Not added to messaging events yet only sending)
  - std::string colour;
  - std::string description;
  - std::string iconURL;
  - std::string media;
  - std::string title;
  - std::string url;

you can get them by calling them like

```cpp
// for this example varName is a Event

varName.message.content;
varName.author.isBot;
varName.message.arguments[0].argument; // you have to use [int] for the vectors obviously
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
  - deleteMessage(channelID, messageID)  // string, string
  - pinMessage(channelID, messageID)     // string, string
  - unpinMessage(channelID, messageID)   // string, string
  - addRole(serverID, roleID)            // string, string (note: this is a w.i.p. and might break unsure)

  - addReaction(channelID, messageID, emojiID) // string, string, string
  - 
  - sendMessage(channelID, message, std::vector<Embed>)      // string, string, vector(optional)

  - getMessage(channelID, messageID, [](Event varName) {}) // string, string, callback lambda
  - getRole(serverID, roleID, [](Event varName) {})        // string, string callback lambda
  - getUser(serverID, userID, [](Event varName) {})                // string, string, callback lambda
  - getEmoji(emojiID, [](Event varName) {})                // string, callback lambda

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

embed example

```cpp
MessageEventsAPI::setEventListener([&](Event event) mutable {
        if (event.type == "Message") {
            if (event.message.isCommand) {
                if (event.message.command == "embedtests") {
                    std::vector<Embed> embeds; // Embed should already be a declared struct

                    Embed embed1;
                    Embed embed2; // supports multiple embeds

                    embed1.title = "embed1";
                    embed1.color = "pink";

                    embed2.title = "embed2 :3";
                    embed2.description = "im a test embed!";
                    
                    RequestsAPI::sendMessage(event.message.channelID, "Testing Embeds", embeds); // just send the embed vector
                }
            }
        }
    });
```

## Events

#### Supported Events: [ones ive tested]
  - MessageReact
  - Message
  - MessageUnreact
  - MessageDelete
  - MessageUpdate

#### All Events: [ones i know exist but havent tested if parsing parses all the data]
  - MessageReact
  - Message
  - MessageUnreact
  - MessageDelete
  - MessageUpdate
  - ChannelStartTyping
  - ChannelStopTyping
  - ServerMemberUpdate
  - Authenticated
  - Ready
  - Pong
  - ServerRoleUpdate
  - EmojiCreate
  - EmojiDelete
  - ServerUpdate
  - ChannelCreate
  - ChannelDelete
  - ChannelUpdate
  - WebhookCreate
  - WebhookDelete
  - ect. pfff i dunno :p

get them by doing varName.type;

So these events you can use any of them as Event passes all types (if it doesnt error)

To do so u can get the raw data like
```cpp
MessageEventsAPI::setEventListener([lastPinID](Event event) mutable {
    std::cout << event.rawData << '\n'; // already with 4 space indentions
    if (event.type == "EmojiCreate") {
        // here u can use any of the struct data it SHOULD parse all events (if they dont error)
        std::cout << event.emoji.ID << '\n';
    }
}
```

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

give/remove roles \[works but if u cant do it more than liek 1 a second or else ull be rate limited\]

banning/kicking

embeds
