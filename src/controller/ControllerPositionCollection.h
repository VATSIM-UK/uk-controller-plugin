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
            bool AddPosition(std::unique_ptr<ControllerPosition> position);
            const ControllerPosition & FetchPositionByCallsign(std::string callsign) const;
            const ControllerPosition & FetchPositionByFacilityTypeAndFrequency(
                std::string facility,
                std::string type,
                double frequency
            ) const;
            size_t GetSize(void) const;
            bool HasPosition(std::string callsign) const;

        private:
            std::map<std::string, std::unique_ptr<ControllerPosition>> positions;
            bool IsPossibleAirfieldPosition(std::string facility) const;
            bool IsPossibleAreaPosition(std::string facility) const;
        };
    }  // namespace Controller
}  // namespace UKControllerPlugin
