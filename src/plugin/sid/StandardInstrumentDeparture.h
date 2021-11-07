#pragma once

namespace UKControllerPlugin::Sid {
    /*
        Represents a standard instrument departure.
    */
    class StandardInstrumentDeparture
    {
        public:
        StandardInstrumentDeparture(
            std::string airfield, std::string identifier, int initialAltitude, int initialHeading, int handoffId);

        [[nodiscard]] auto Airfield() const -> std::string;
        [[nodiscard]] auto Identifier() const -> std::string;
        [[nodiscard]] auto InitialAltitude() const -> int;
        [[nodiscard]] auto InitialHeading() const -> int;
        [[nodiscard]] auto HandoffId() const -> int;
        [[nodiscard]] auto HasHandoff() const -> bool;

        private:
        const std::string airfield;
        const std::string identifier;
        const int initialAltitude;
        const int initialHeading;
        const int handoffId;
    };
} // namespace UKControllerPlugin::Sid
