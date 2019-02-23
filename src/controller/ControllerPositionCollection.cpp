#include "pch/stdafx.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/TranslateFrequencyAbbreviation.h"

using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPlugin {
    namespace Controller {

        /*
            Inserts a position into the collection, if the callsign isn't already added.
        */
        bool ControllerPositionCollection::AddPosition(std::unique_ptr<ControllerPosition> position)
        {
            return this->positions.insert({ position->GetCallsign(), std::move(position) }).second;
        }

        /*
            Retrieves a position by an exact callsign match alone.
        */
        const ControllerPosition & ControllerPositionCollection::FetchPositionByCallsign(std::string callsign) const
        {
            auto position = this->positions.find(callsign);
            if (position == this->positions.end()) {
                throw std::out_of_range("Position " + callsign + " not found.");
            }

            return *position->second;
        }

        /*
            Retrieves a position based on it's facility (EGXX, LON, LTC, SCO etc) and frequency.
        */
        const ControllerPosition & ControllerPositionCollection::FetchPositionByFacilityAndFrequency(
            std::string facility,
            double frequency
        ) const {

            facility = TranslateFrequencyAbbreviation(facility);

            // If there's no chance of finding anything, save us the iteration.
            if (!this->IsPossibleAreaPosition(facility) && !this->IsPossibleAirfieldPosition(facility)) {
                throw std::out_of_range("Position not found.");
            }

            // Iterate through the positions and try to match.
            auto position = std::find_if(this->positions.begin(), this->positions.end(),
                [facility, frequency]
                (std::pair<std::string, const std::unique_ptr<ControllerPosition> &> position) -> bool {

                // Frequency matching is done to 4dp, because floating points.
                return fabs(frequency - position.second->GetFrequency()) < 0.001 &&
                    position.first.substr(0, position.first.find('_')).compare(facility) == 0;
            });

            if (position == this->positions.end()) {
                throw std::out_of_range("Position not found.");
            }

            return *position->second;
        }

        /*
            Returns the number of elements in the collection.
        */
        size_t ControllerPositionCollection::GetSize(void) const
        {
            return this->positions.size();
        }

        /*
            Returns true if the facility is possible for an airfield.
        */
        bool ControllerPositionCollection::IsPossibleAirfieldPosition(std::string facility) const
        {
            return facility == "ESSEX" || facility == "THAMES" || facility == "SOLENT" || 
                facility.size() == 4 && facility.substr(0, 2) == "EG";
        }

        /*
            Returns true if the facility is possible for an area postion.
        */
        bool ControllerPositionCollection::IsPossibleAreaPosition(std::string facility) const
        {
            return facility == "LON" || facility == "LTC" ||
                facility == "SCO" || facility == "STC" || facility == "MAN";
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin
