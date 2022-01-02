#pragma once

namespace UKControllerPlugin::Airfield {
    class AirfieldCollection;
} // namespace UKControllerPlugin::Airfield

namespace UKControllerPlugin::Metar {
    class ParsedMetar;
    class MetarComponentsFactory;

    /**
     * Builds a ParsedMetar from JSON
     */
    class ParsedMetarFactory
    {
        public:
        ParsedMetarFactory(
            const MetarComponentsFactory& componentsFactory, const Airfield::AirfieldCollection& airfields);
        [[nodiscard]] auto FromJson(const nlohmann::json& metarData) const -> std::shared_ptr<ParsedMetar>;

        private:
        [[nodiscard]] auto MessageValid(const nlohmann::json& message) const -> bool;

        // Builds individual METAR components
        const MetarComponentsFactory& componentsFactory;

        // All the airfields
        const Airfield::AirfieldCollection& airfields;
    };
} // namespace UKControllerPlugin::Metar
