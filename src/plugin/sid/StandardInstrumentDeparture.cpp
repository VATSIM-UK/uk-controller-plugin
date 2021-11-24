#include "StandardInstrumentDeparture.h"

namespace UKControllerPlugin::Sid {

    StandardInstrumentDeparture::StandardInstrumentDeparture(
        std::string airfield,
        std::string identifier,
        int initialAltitude,
        int initialHeading,
        int handoffId,
        std::set<int> prenotes)
        : airfield(std::move(airfield)), identifier(std::move(identifier)), initialAltitude(initialAltitude),
          initialHeading(initialHeading), handoffId(handoffId), prenotes(std::move(prenotes))
    {
    }

    auto StandardInstrumentDeparture::Airfield() const -> std::string
    {
        return this->airfield;
    }

    auto StandardInstrumentDeparture::Identifier() const -> std::string
    {
        return this->identifier;
    }

    auto StandardInstrumentDeparture::InitialAltitude() const -> int
    {
        return this->initialAltitude;
    }

    auto StandardInstrumentDeparture::InitialHeading() const -> int
    {
        return this->initialHeading;
    }

    auto StandardInstrumentDeparture::HandoffId() const -> int
    {
        return this->handoffId;
    }

    auto StandardInstrumentDeparture::HasHandoff() const -> bool
    {
        return this->handoffId != 0;
    }

    auto StandardInstrumentDeparture::Prenotes() const -> const std::set<int>&
    {
        return this->prenotes;
    }
} // namespace UKControllerPlugin::Sid
