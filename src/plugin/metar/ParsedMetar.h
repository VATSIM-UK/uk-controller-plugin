#pragma once
#include "MetarComponents.h"

namespace UKControllerPlugin::Metar {
    struct MetarComponents;
    class PressureComponent;

    /**
     * A METAR, that has been parsed into its individual components.
     */
    class ParsedMetar
    {
        public:
        ParsedMetar(std::string airfield, std::string raw, std::unique_ptr<MetarComponents> components);
        ~ParsedMetar();
        ParsedMetar(const ParsedMetar&) = delete;
        ParsedMetar(ParsedMetar&&) noexcept;
        auto operator=(const ParsedMetar&) -> ParsedMetar& = delete;
        auto operator=(ParsedMetar&&) noexcept -> ParsedMetar& = delete;

        [[nodiscard]] auto Airfield() const -> const std::string&;
        [[nodiscard]] auto Raw() const -> const std::string&;
        [[nodiscard]] auto Components() const -> const MetarComponents&;

        private:
        // The airfield for the METAR
        const std::string airfield;

        // The raw METAR
        const std::string raw;

        // The different components of the METAR
        std::unique_ptr<MetarComponents> components;
    };
} // namespace UKControllerPlugin::Metar
