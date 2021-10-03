#pragma once

namespace UKControllerPlugin::Controller {

    /*
        Class that represents a potential UK Controller position.
    */
    class ControllerPosition
    {
        public:
        ControllerPosition(
            int id,
            std::string callsign,
            double frequency,
            std::vector<std::string> topdown,
            bool requestsDepartureReleases,
            bool receivesDepartureReleases,
            bool sendsPrenoteMessages = false,
            bool receivesPrenoteMessages = false);

        [[nodiscard]] auto GetId() const -> int;
        [[nodiscard]] auto GetUnit() const -> std::string;
        [[nodiscard]] auto GetCallsign() const -> std::string;
        [[nodiscard]] auto GetFrequency() const -> double;
        [[nodiscard]] auto GetTopdown() const -> std::vector<std::string>;
        [[nodiscard]] auto GetType() const -> std::string;
        [[nodiscard]] auto IsDelivery() const -> bool;
        [[nodiscard]] auto IsGround() const -> bool;
        [[nodiscard]] auto IsTower() const -> bool;
        [[nodiscard]] auto IsApproach() const -> bool;
        [[nodiscard]] auto IsEnroute() const -> bool;
        [[nodiscard]] auto IsFlightServiceStation() const -> bool;
        [[nodiscard]] auto ProvidesGroundServices() const -> bool;
        [[nodiscard]] auto ProvidesTowerServices() const -> bool;
        [[nodiscard]] auto ProvidesApproachServices() const -> bool;
        [[nodiscard]] auto HasTopdownAirfield(const std::string& icao) const -> bool;
        [[nodiscard]] auto RequestsDepartureReleases() const -> bool;
        [[nodiscard]] auto ReceivesDepartureReleases() const -> bool;
        [[nodiscard]] auto SendsPrenoteMessages() const -> bool;
        [[nodiscard]] auto ReceivesPrenoteMessages() const -> bool;
        auto operator==(const ControllerPosition& position) const -> bool;

        private:
        // The id of the controller in the API
        const int id;

        // The default controller callsign
        const std::string callsign;

        // The controller frequency
        const double frequency;

        // The airfields for which this controller has some level of top-down responsibility.
        const std::vector<std::string> topdown;

        // Can the controller request a departure release
        const bool requestsDepartureReleases;

        // Can the controller receive a departure release
        const bool receivesDepartureReleases;

        // Can this controller send a prenote
        const bool sendsPrenoteMessages;

        // Can this controller cancel a prenote
        const bool receivesPrenoteMessages;

        const double frequencyMatchDelta = 0.001;
    };
} // namespace UKControllerPlugin::Controller
