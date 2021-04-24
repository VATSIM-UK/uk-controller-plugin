#pragma once

namespace UKControllerPlugin {
    namespace Sid {
        /*
            Represents a standard instrument departure.
        */
        class StandardInstrumentDeparture
        {
            public:

                StandardInstrumentDeparture(
                    std::string airfield,
                    std::string identifier,
                    int initialAltitude,
                    int initialHeading
                );

                std::string Airfield() const;
                std::string Identifier() const;
                int InitialAltitude() const;
                int InitialHeading() const;
            private:

                const std::string airfield;
                const std::string identifier;
                const int initialAltitude;
                const int initialHeading;
        };
    }  // namespace Sid
}  // namespace UKControllerPlugin
