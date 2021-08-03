#include "pch/pch.h"
#include "sid/StandardInstrumentDeparture.h"

namespace UKControllerPlugin {
    namespace Sid {

        StandardInstrumentDeparture::StandardInstrumentDeparture(
            std::string airfield,
            std::string identifier,
            int initialAltitude,
            int initialHeading
        ) : airfield(std::move(airfield)), identifier(std::move(identifier)),
            initialAltitude(initialAltitude), initialHeading(initialHeading) {}

        std::string StandardInstrumentDeparture::Airfield() const
        {
            return this->airfield;
        }

        std::string StandardInstrumentDeparture::Identifier() const
        {
            return this->identifier;
        }

        int StandardInstrumentDeparture::InitialAltitude() const
        {
            return this->initialAltitude;
        }

        int StandardInstrumentDeparture::InitialHeading() const
        {
            return this->initialHeading;
        }
    }  // namespace Sid
}  // namespace UKControllerPlugin
