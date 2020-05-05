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
        const ControllerPosition & ControllerPositionCollection::FetchPositionByFacilityTypeAndFrequency(
            std::string facility,
            std::string type,
            double frequency
        ) const {

            facility = TranslateFrequencyAbbreviation(facility);

            // Iterate through the positions and try to match.
            auto position = std::find_if(this->positions.begin(), this->positions.end(),
                [facility, frequency, type]
                (std::pair<std::string, const std::unique_ptr<ControllerPosition> &> position) -> bool {

                // Frequency matching is done to 4dp, because floating points.
                return fabs(frequency - position.second->GetFrequency()) < 0.001 &&
                    position.second->GetUnit() == facility &&
                    position.second->GetType() == type;
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

        bool ControllerPositionCollection::HasPosition(std::string callsign) const
        {
            return this->positions.find(callsign) != this->positions.cend();
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin
