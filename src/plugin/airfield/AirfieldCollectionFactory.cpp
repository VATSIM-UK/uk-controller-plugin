#include "AirfieldCollection.h"
#include "AirfieldCollectionFactory.h"
#include "airfield/AirfieldModel.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPositionHierarchyFactory.h"

using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;

namespace UKControllerPlugin::Airfield {

    auto
    CreateAirfieldCollection(const nlohmann::json& airfields, const ControllerPositionHierarchyFactory& hierarchyFactory)
        -> std::unique_ptr<const UKControllerPlugin::Airfield::AirfieldCollection>
    {
        auto collection = std::make_unique<AirfieldCollection>();

        if (!airfields.is_array()) {
            LogError("Invalid airfield data");
            return collection;
        }

        for (const auto& airfield : airfields) {
            if (!AirfieldValid(airfield, hierarchyFactory)) {
                LogWarning("Invalid airfield detected: " + airfield.dump());
                continue;
            }

            collection->AddAirfield(std::make_shared<AirfieldModel>(
                airfield.at("id").get<int>(),
                airfield.at("identifier").get<std::string>(),
                hierarchyFactory.CreateFromJsonById(airfield.at("top_down_controller_positions"))));
        }

        LogInfo("Loaded " + std::to_string(collection->GetSize()) + " airfields");
        return collection;
    }

    auto AirfieldValid(const nlohmann::json& airfield, const ControllerPositionHierarchyFactory& hierarchyFactory)
        -> bool
    {
        return airfield.is_object() && airfield.contains("id") && airfield.at("id").is_number_integer() &&
               airfield.contains("identifier") && airfield.at("identifier").is_string() &&
               airfield.contains("top_down_controller_positions") &&
               airfield.at("top_down_controller_positions").is_array() &&
               !airfield.at("top_down_controller_positions").empty() &&
               hierarchyFactory.CreateFromJsonById(airfield.at("top_down_controller_positions")) != nullptr;
    }
} // namespace UKControllerPlugin::Airfield
