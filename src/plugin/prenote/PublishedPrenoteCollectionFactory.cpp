#include "PublishedPrenote.h"
#include "PublishedPrenoteCollectionFactory.h"
#include "PublishedPrenoteCollection.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPositionHierarchyFactory.h"

namespace UKControllerPlugin::Prenote {
    auto CreatePublishedPrenoteCollection(
        const nlohmann::json& prenotes, const Controller::ControllerPositionHierarchyFactory& hierarchyFactory)
        -> std::unique_ptr<PublishedPrenoteCollection>
    {
        auto collection = std::make_unique<PublishedPrenoteCollection>();
        if (!prenotes.is_array()) {
            LogError("Published prenotes are not an array");
            return collection;
        }

        for (const auto& prenote : prenotes) {
            if (!PrenoteValid(prenote, hierarchyFactory)) {
                LogWarning("Invalid published prenote");
                continue;
            }

            collection->Add(std::make_shared<PublishedPrenote>(
                prenote.at("id").get<int>(),
                prenote.at("key").get<std::string>(),
                hierarchyFactory.CreateFromJsonById(prenote.at("controller_positions"))));
        }

        return collection;
    }

    auto
    PrenoteValid(const nlohmann::json& prenote, const Controller::ControllerPositionHierarchyFactory& hierarchyFactory)
        -> bool
    {
        return prenote.is_object() && prenote.contains("id") && prenote.at("id").is_number_integer() &&
               prenote.contains("key") && prenote.at("key").is_string() && prenote.contains("controller_positions") &&
               prenote.at("controller_positions").is_array() && !prenote.at("controller_positions").empty() &&
               hierarchyFactory.CreateFromJsonById(prenote.at("controller_positions")) != nullptr;
    }
} // namespace UKControllerPlugin::Prenote
