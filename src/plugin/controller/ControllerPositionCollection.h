#pragma once

namespace UKControllerPlugin::Controller {
    class ControllerPosition;

    /*
        A collection of all the UK Controller Positions, which can be searched
        to retrieve a specific controller position.
    */
    class ControllerPositionCollection
    {
        public:
        bool AddPosition(const std::shared_ptr<ControllerPosition>& position);
        [[nodiscard]] auto FetchPositionById(int id) const -> std::shared_ptr<ControllerPosition>;
        [[nodiscard]] auto FetchPositionByCallsign(const std::string& callsign) const
            -> std::shared_ptr<ControllerPosition>;
        [[nodiscard]] auto
        FetchPositionByFacilityTypeAndFrequency(std::string facility, const std::string& type, double frequency) const
            -> std::shared_ptr<ControllerPosition>;
        [[nodiscard]] auto GetSize() const -> size_t;

        private:
        std::map<std::string, std::shared_ptr<ControllerPosition>> positions;
        std::map<int, std::shared_ptr<ControllerPosition>> positionsById;

        static inline const double FREQUENCY_MATCH_DELTA = 0.001;
    };
} // namespace UKControllerPlugin::Controller
