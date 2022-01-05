#include "MetarComponents.h"
#include "MetarComponentsFactory.h"
#include "ParsedMetar.h"
#include "ParsedMetarFactory.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"

namespace UKControllerPlugin::Metar {

    ParsedMetarFactory::ParsedMetarFactory(
        const MetarComponentsFactory& componentsFactory, const Airfield::AirfieldCollection& airfields)
        : componentsFactory(componentsFactory), airfields(airfields)
    {
    }

    auto ParsedMetarFactory::FromJson(const nlohmann::json& metarData) const -> std::shared_ptr<ParsedMetar>
    {
        if (!MessageValid(metarData)) {
            LogError("Metar json is not valid");
            return nullptr;
        }

        return std::make_shared<ParsedMetar>(
            airfields.FetchById(metarData.at("airfield_id").get<int>())->Icao(),
            metarData.at("raw").get<std::string>(),
            componentsFactory.FromJson(metarData.at("parsed")));
    }

    auto ParsedMetarFactory::MessageValid(const nlohmann::json& message) const -> bool
    {
        return message.is_object() && message.contains("airfield_id") &&
               message.at("airfield_id").is_number_integer() &&
               airfields.FetchById(message.at("airfield_id").get<int>()) != nullptr && message.contains("raw") &&
               message.at("raw").is_string() && message.contains("parsed") && message.at("parsed").is_object();
    }
} // namespace UKControllerPlugin::Metar
