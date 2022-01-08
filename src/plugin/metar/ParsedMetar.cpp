#include "MetarComponents.h"
#include "ParsedMetar.h"

namespace UKControllerPlugin::Metar {

    ParsedMetar::ParsedMetar(std::string airfield, std::string raw, std::unique_ptr<MetarComponents> components)
        : airfield(std::move(airfield)), raw(std::move(raw)), components(std::move(components))
    {
    }

    ParsedMetar::~ParsedMetar() = default;
    ParsedMetar::ParsedMetar(ParsedMetar&&) noexcept = default;

    auto ParsedMetar::Airfield() const -> const std::string&
    {
        return airfield;
    }

    auto ParsedMetar::Raw() const -> const std::string&
    {
        return raw;
    }

    auto ParsedMetar::Components() const -> const MetarComponents&
    {
        return *components;
    }
} // namespace UKControllerPlugin::Metar
