#pragma once

namespace UKControllerPlugin {
    namespace Controller {

        /*
            Class that repsents a potential UK Controller position.
        */
        class ControllerPosition
        {
            public:
                ControllerPosition(
                    std::string callsign,
                    double frequency,
                    std::string type,
                    std::vector<std::string> topdown
                );
                std::string GetUnit(void) const;
                std::string GetCallsign(void) const;
                double GetFrequency(void) const;
                std::vector<std::string> GetTopdown(void) const;
                std::string GetType(void) const;
                bool HasTopdownAirfield(std::string icao) const;
                bool operator==(const ControllerPosition & position) const;

            private:
                // The default controller callsign
                std::string callsign;

                // The controller frequency
                double frequency;

                // The type of position e.g. APP, CTR
                std::string type;

                // The airfields for which this controller has some level of top-down responsibility.
                std::vector<std::string> topdown;
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin
