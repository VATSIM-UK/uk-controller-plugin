#include "pch/stdafx.h"
#include "push/InterpretPushEvent.h"

using UKControllerPlugin::Push::PushEvent;

PushEvent InterpretPushedEvent(std::string message)
{
    nlohmann::json messageJson;
    try {
        messageJson = nlohmann::json::parse(message);
    } catch (nlohmann::json::exception) {
        LogWarning("Pushed event is not valid JSON: " + message);
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
                LogWarning("Pushed event data field is not an object");
            }

            dataJson = parsedData.is_object() ? parsedData : nlohmann::json();
        }
    } catch (nlohmann::json::exception) {
        LogWarning("Invalid pushed event data field");
    }

    return PushEvent{
        messageJson.at("event"),
        messageJson.count("channel") && messageJson.at("channel").is_string() ? messageJson.at("channel") : "none",
        dataJson,
        message
    };
}
