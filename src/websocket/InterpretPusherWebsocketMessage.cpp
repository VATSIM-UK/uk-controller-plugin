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

    nlohmann::json dataJson;
    try {
        if (messageJson.count("data")) {

            nlohmann::json parsedData = nlohmann::json::parse(messageJson.at("data").get<std::string>());

            if (!parsedData.is_object()) {
                LogWarning("Pusher data field is not an object");
            }

            dataJson = parsedData.is_object() ? parsedData : nlohmann::json();
        }
    } catch (nlohmann::json::exception) {
        LogWarning("Invalid pusher data field");
    }

    return WebsocketMessage{
        messageJson.at("event"),
        messageJson.count("channel") && messageJson.at("channel").is_string() ? messageJson.at("channel") : "none",
        dataJson,
        message,
        messageJson.at("event").get<std::string>().substr(0, 6) == "pusher"
    };
}
