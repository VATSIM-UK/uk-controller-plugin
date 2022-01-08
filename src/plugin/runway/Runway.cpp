#include "Runway.h"

namespace UKControllerPlugin::Runway {

    Runway::Runway(int id, int airfieldId, int heading, EuroScopePlugIn::CPosition threshold)
        : id(id), airfieldId(airfieldId), heading(heading), threshold(threshold)
    {
    }

    auto Runway::Id() const -> int
    {
        return id;
    }

    auto Runway::AirfieldId() const -> int
    {
        return airfieldId;
    }

    auto Runway::Identifier() const -> const std::string&
    {
        return identifier;
    }

    auto Runway::Heading() const -> int
    {
        return heading;
    }

    auto Runway::Threshold() const -> const EuroScopePlugIn::CPosition&
    {
        return threshold;
    }
} // namespace UKControllerPlugin::Runway
