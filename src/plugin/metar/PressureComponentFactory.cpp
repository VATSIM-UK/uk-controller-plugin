#include "MetarComponents.h"
#include "PressureComponent.h"
#include "PressureComponentFactory.h"

namespace UKControllerPlugin::Metar {

    void PressureComponentFactory::FromJson(const nlohmann::json& json, MetarComponents& components) const
    {
        if (!JsonValid(json)) {
            LogError("Pressure component of metar is invalid: " + json.dump());
            return;
        }

        components.pressure = std::make_shared<PressureComponent>(
            json.at("qnh").get<int>(),
            json.at("qfe").get<int>(),
            json.at("qnh_inhg").get<float>(),
            json.at("qfe_inhg").get<float>(),
            json.at("pressure_format").get<std::string>() == "hpa" ? PressureUnit::Hectopascals : PressureUnit::InHg);
    }

    auto PressureComponentFactory::JsonValid(const nlohmann::json& json) const -> bool
    {
        return PressureFormatValid(json) && HectopascalPressuresValid(json) && InHgPressuresValid(json);
    }

    auto PressureComponentFactory::PressureFormatValid(const nlohmann::json& json) const -> bool
    {
        return json.contains("pressure_format") && json.at("pressure_format").is_string() &&
               PressureFormatValidUnit(json.at("pressure_format").get<std::string>());
    }

    auto PressureComponentFactory::PressureFormatValidUnit(const std::string& pressureFormat) -> bool
    {
        return pressureFormat == "hpa" || pressureFormat == "inhg";
    }

    auto PressureComponentFactory::InHgPressuresValid(const nlohmann::json& json) -> bool
    {
        return json.contains("qnh_inhg") && json.at("qnh_inhg").is_number() && json.contains("qfe_inhg") &&
               json.at("qfe_inhg").is_number();
    }

    auto PressureComponentFactory::HectopascalPressuresValid(const nlohmann::json& json) -> bool
    {
        return json.contains("qnh") && json.at("qnh").is_number_integer() && json.contains("qfe") &&
               json.at("qfe").is_number_integer();
    }
} // namespace UKControllerPlugin::Metar
