#pragma once

namespace UKControllerPlugin::Sid {
    /*
        Represents a standard instrument departure.
    */
    class StandardInstrumentDeparture
    {
        public:
        StandardInstrumentDeparture(
            int id,
            int runwayId,
            std::string identifier,
            int initialAltitude,
            int initialHeading,
            int handoffId,
            std::set<int> prenotes = {});

        [[nodiscard]] auto Id() const -> int;
        [[nodiscard]] auto RunwayId() const -> int;
        [[nodiscard]] auto Identifier() const -> std::string;
        [[nodiscard]] auto InitialAltitude() const -> int;
        [[nodiscard]] auto InitialHeading() const -> int;
        [[nodiscard]] auto HandoffId() const -> int;
        [[nodiscard]] auto HasHandoff() const -> bool;
        [[nodiscard]] auto Prenotes() const -> const std::set<int>&;

        private:
        const int id;
        const int runwayId;
        const std::string identifier;
        const int initialAltitude;
        const int initialHeading;
        const int handoffId;
        const std::set<int> prenotes;
    };
} // namespace UKControllerPlugin::Sid
