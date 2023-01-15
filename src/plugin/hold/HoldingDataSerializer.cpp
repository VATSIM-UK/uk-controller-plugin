#include "AbstractHoldLevelRestriction.h"
#include "DeemedSeparatedHold.h"
#include "DeemedSeparatedHoldSerializer.h"
#include "HoldingData.h"
#include "HoldingDataSerializer.h"
#include "HoldRestrictionSerializer.h"
#include "bootstrap/PersistenceContainer.h"
#include "geometry/MeasurementUnitType.h"
#include "geometry/MeasurementUnitFactory.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin::Hold {

    const HoldingData holdSerializerInvalid = {0, "INVALID"}; // NOLINT

    /*
        Create HoldingData from JSON
    */
    void from_json(const nlohmann::json& json, HoldingData& holdingData)
    {
        if (!JsonValid(json)) {
            holdingData = {0, "INVALID"};
            return;
        }

        json.at("id").get_to(holdingData.identifier);
        json.at("fix").get_to(holdingData.fix);
        json.at("description").get_to(holdingData.description);
        json.at("minimum_altitude").get_to(holdingData.minimum);
        json.at("maximum_altitude").get_to(holdingData.maximum);
        json.at("inbound_heading").get_to(holdingData.inbound);
        json.at("turn_direction").get_to(holdingData.turnDirection);

        if (!json.at("outbound_leg_unit").is_null()) {
            holdingData.outboundLeg = std::make_unique<Geometry::Measurement>(
                Geometry::UnitFromString(json.at("outbound_leg_unit").get<std::string>()),
                json.at("outbound_leg_value").get<double>());
        }
    }

    void from_json_with_restrictions(
        const nlohmann::json& json, HoldingData& holdingData, const PersistenceContainer& container)
    {
        from_json(json, holdingData);

        if (holdingData == holdSerializerInvalid) {
            return;
        }

        std::set<std::unique_ptr<AbstractHoldLevelRestriction>> restrictions;
        hold_restriction_from_json(json.at("restrictions"), restrictions, container);
        holdingData.restrictions = std::move(restrictions);

        std::set<std::unique_ptr<DeemedSeparatedHold>> deemedSeparatedHolds;
        deemed_separated_from_json(json.at("deemed_separated_holds"), deemedSeparatedHolds);
        holdingData.deemedSeparatedHolds = std::move(deemedSeparatedHolds);
    }

    auto JsonValid(const nlohmann::json& data) -> bool
    {
        return data.is_object() && data.find("id") != data.end() && data.at("id").is_number_integer() &&
               data.find("fix") != data.end() && data.at("fix").is_string() && data.find("description") != data.end() &&
               data.at("description").is_string() && data.find("minimum_altitude") != data.end() &&
               data.at("minimum_altitude").is_number_integer() && data.find("maximum_altitude") != data.end() &&
               data.at("maximum_altitude").is_number_integer() && data.find("inbound_heading") != data.end() &&
               data.at("inbound_heading").is_number_integer() && data.find("turn_direction") != data.end() &&
               data.at("turn_direction").is_string() &&
               (data.at("turn_direction") == "left" || data.at("turn_direction") == "right") &&
               data.find("restrictions") != data.end() && data.at("restrictions").is_array() &&
               data.contains("deemed_separated_holds") && data.at("deemed_separated_holds").is_array() &&
               data.contains("outbound_leg_value") && data.contains("outbound_leg_unit") &&
               (data.at("outbound_leg_unit").is_null() ||
                (data.at("outbound_leg_value").is_number() && data.at("outbound_leg_unit").is_string() &&
                 Geometry::UnitStringValid(data.at("outbound_leg_unit").get<std::string>())));
    }
} // namespace UKControllerPlugin::Hold
