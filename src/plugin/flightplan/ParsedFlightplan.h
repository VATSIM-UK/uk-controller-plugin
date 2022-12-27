#pragma once

namespace UKControllerPlugin::Flightplan {

    class FlightplanPoint;

    /*
        Represents points on a flightplan
    */
    class ParsedFlightplan
    {
        public:
        void AddPoint(const std::shared_ptr<FlightplanPoint>& point);
        [[nodiscard]] auto CountPoints() const -> size_t;
        [[nodiscard]] auto HasPointByIdentifier(const std::string& identifier) const -> bool;
        [[nodiscard]] auto PointByIndex(int index) const -> std::shared_ptr<FlightplanPoint>;

        private:
        // The points, by index
        std::map<int, std::shared_ptr<FlightplanPoint>> pointsByIndex;

        // The points by identifier
        std::map<std::string, std::set<int>> pointsByIdentifier;
    };
} // namespace UKControllerPlugin::Flightplan
