#pragma once

namespace UKControllerPlugin {
    namespace Controller {

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
                    bool receivesDepartureReleases
                );

                int GetId() const;
                std::string GetUnit() const;
                std::string GetCallsign() const;
                double GetFrequency() const;
                std::vector<std::string> GetTopdown() const;
                std::string GetType() const;
                bool HasTopdownAirfield(std::string icao) const;
                bool RequestsDepartureReleases() const;
                bool ReceivesDepartureReleases() const;
                bool operator==(const ControllerPosition & position) const;

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
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin
