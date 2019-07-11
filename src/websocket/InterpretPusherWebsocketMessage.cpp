#include "pch/stdafx.h"
#include "websocket/InterpretPusherWebsocketMessage.h"

using UKControllerPlugin::Websocket::WebsocketMessage;

WebsocketMessage InterpretPusherMessage(std::string message)
{
    nlohmann::json messageJson;
    try {
        messageJson = nlohmann::json::parse(message);
    } catch (nlohmann::json::exception) {
        LogWarning("Websocket message is not valid JSON: " + message);
        return invalidMessage;
    }

    if (!messageJson.count("event") || !messageJson.at("event").is_string()) {
        return invalidMessage;
    }

    std::string test = messageJson.at("event").get<std::string>().substr(0, 6);
    return WebsocketMessage{
        messageJson.at("event"),
        messageJson.count("channel") && messageJson.at("channel").is_string() ? messageJson.at("channel") : "none",
        messageJson.count("data") && messageJson.at("data").is_object() ? messageJson.at("data") : nlohmann::json(),
        messageJson.at("event").get<std::string>().substr(0, 6) == "pusher"
    };
}
