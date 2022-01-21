#include "StandardInstrumentDeparture.h"

namespace UKControllerPlugin::Sid {

    StandardInstrumentDeparture::StandardInstrumentDeparture(
        int id,
        int runwayId,
        std::string identifier,
        int initialAltitude,
        int initialHeading,
        int handoffId,
        std::set<int> prenotes)
        : id(id), runwayId(runwayId), identifier(std::move(identifier)), initialAltitude(initialAltitude),
          initialHeading(initialHeading), handoffId(handoffId), prenotes(std::move(prenotes))
    {
    }

    auto StandardInstrumentDeparture::Id() const -> int
    {
        return this->id;
    }

    auto StandardInstrumentDeparture::RunwayId() const -> int
    {
        return this->runwayId;
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
