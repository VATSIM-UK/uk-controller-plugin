#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroscopeCoordinateInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Flightplan {

    /*
        A point in a flightplan
    */
    class FlightplanPoint
    {
        public:
        FlightplanPoint(
            int index, std::string identifier, std::shared_ptr<Euroscope::EuroscopeCoordinateInterface> position);
        ~FlightplanPoint() = default;
        [[nodiscard]] auto Index() const -> int;
        [[nodiscard]] auto Identifier() const -> const std::string&;
        [[nodiscard]] auto Position() const -> const Euroscope::EuroscopeCoordinateInterface&;

        private:
        // The index of the point in the flightplan
        int index;

        // The identifier of the point
        std::string identifier;

        // The position of the point
        std::shared_ptr<Euroscope::EuroscopeCoordinateInterface> position;
    };
} // namespace UKControllerPlugin::Flightplan
