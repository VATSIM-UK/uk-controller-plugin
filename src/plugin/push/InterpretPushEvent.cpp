#include "push/InterpretPushEvent.h"

using UKControllerPlugin::Push::PushEvent;

namespace UKControllerPlugin::Push {
    PushEvent InterpretPushedEvent(std::string message)
    {
        nlohmann::json messageJson;
        try {
            messageJson = nlohmann::json::parse(message);
        } catch (nlohmann::json::exception) {
            LogWarning("Pushed event is not valid JSON: " + message);
            return invalidMessage;
        }

        if (!messageJson.contains("event") || !messageJson.at("event").is_string()) {
            return invalidMessage;
        }

        if (!messageJson.contains("data") || !messageJson.at("data").is_object()) {
            return invalidMessage;
        }

        return PushEvent{
            messageJson.at("event"),
            messageJson.count("channel") && messageJson.at("channel").is_string() ? messageJson.at("channel") : "none",
            messageJson.at("data"),
            message};
    }
} // namespace UKControllerPlugin::Push
