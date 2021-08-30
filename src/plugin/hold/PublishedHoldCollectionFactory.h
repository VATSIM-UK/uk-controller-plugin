#pragma once
#include "bootstrap/PersistenceContainer.h"
#include "hold/HoldManager.h"

namespace UKControllerPlugin::Hold {

    auto CreatePublishedHoldCollection(
        const nlohmann::json& data, const UKControllerPlugin::Bootstrap::PersistenceContainer& container)
        -> std::unique_ptr<UKControllerPlugin::Hold::PublishedHoldCollection>;
} // namespace UKControllerPlugin::Hold
