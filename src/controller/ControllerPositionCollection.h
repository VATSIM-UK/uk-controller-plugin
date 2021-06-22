#pragma once

namespace UKControllerPlugin {
    namespace Controller {

        // Forward Declarations
        class ControllerPosition;
        // END

        /*
            A collection of all the UK Controller Positions, which can be searched
            to retrieve a specific controller position.
        */
        class ControllerPositionCollection
        {
        public:
                bool AddPosition(std::shared_ptr<ControllerPosition> position);
                const std::shared_ptr<ControllerPosition> FetchPositionById(int id) const;
                const ControllerPosition& FetchPositionByCallsign(std::string callsign) const;
                const ControllerPosition& FetchPositionByFacilityTypeAndFrequency(
                    std::string facility,
                    std::string type,
                    double frequency
                ) const;
                size_t GetSize(void) const;
                bool HasPosition(std::string callsign) const;

        private:
                std::map<std::string, std::shared_ptr<ControllerPosition>> positions;
                std::map<int, std::shared_ptr<ControllerPosition>> positionsById;
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin
