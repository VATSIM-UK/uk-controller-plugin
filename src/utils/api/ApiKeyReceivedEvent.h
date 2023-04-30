#pragma once

namespace UKControllerPluginUtils::Api {
    using ApiKeyReceivedEvent = struct ApiKeyReceivedEvent
    {
        ApiKeyReceivedEvent(const std::string& key) : key(key)
        {
        }

        const std::string key;
    };
} // namespace UKControllerPluginUtils::Api
