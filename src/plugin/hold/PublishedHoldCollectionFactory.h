#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin::Hold {
    class HoldManager;
    class PublishedHoldCollection;

    auto CreatePublishedHoldCollection(
        const nlohmann::json& data, const UKControllerPlugin::Bootstrap::PersistenceContainer& container)
        -> std::unique_ptr<PublishedHoldCollection>;
} // namespace UKControllerPlugin::Hold
