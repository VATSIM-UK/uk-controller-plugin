#include "FlightplanPoint.h"
#include "euroscope/EuroscopeCoordinateInterface.h"

namespace UKControllerPlugin::Flightplan {
    FlightplanPoint::FlightplanPoint(
        int index, std::string identifier, std::shared_ptr<Euroscope::EuroscopeCoordinateInterface> position)
        : index(index), identifier(std::move(identifier)), position(std::move(position))
    {
    }

    auto FlightplanPoint::Index() const -> int
    {
        return index;
    }

    auto FlightplanPoint::Identifier() const -> const std::string&
    {
        return identifier;
    }

    auto FlightplanPoint::Position() const -> const Euroscope::EuroscopeCoordinateInterface&
    {
        return *position;
    }

} // namespace UKControllerPlugin::Flightplan
