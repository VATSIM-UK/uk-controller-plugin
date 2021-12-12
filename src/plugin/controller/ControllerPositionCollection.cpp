#include "ControllerPosition.h"
#include "ControllerPositionCollection.h"
#include "TranslateFrequencyAbbreviation.h"

using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPlugin::Controller {

    /*
        Inserts a position into the collection, if the callsign isn't already added.
    */
    auto ControllerPositionCollection::AddPosition(const std::shared_ptr<ControllerPosition>& position) -> bool
    {
        return this->positions.insert({position->GetCallsign(), position}).second &&
               this->positionsById.insert({position->GetId(), position}).second;
    }

    /*
     * Fetch a position by id
     */
    auto ControllerPositionCollection::FetchPositionById(int id) const -> std::shared_ptr<ControllerPosition>
    {
        auto position = this->positionsById.find(id);
        return position == this->positionsById.end() ? nullptr : position->second;
    }

    /*
        Retrieves a position by an exact callsign match alone.
    */
    auto ControllerPositionCollection::FetchPositionByCallsign(const std::string& callsign) const
        -> std::shared_ptr<ControllerPosition>
    {
        auto position = this->positions.find(callsign);
        return position == this->positions.cend() ? nullptr : position->second;
    }

    /*
        Retrieves a position based on it's facility (EGXX, LON, LTC, SCO etc) and frequency.
    */
    auto ControllerPositionCollection::FetchPositionByFacilityTypeAndFrequency(
        std::string facility, const std::string& type, double frequency) const -> std::shared_ptr<ControllerPosition>
    {

        facility = TranslateFrequencyAbbreviation(facility);

        // Iterate through the positions and try to match.
        auto position = std::find_if(
            this->positions.begin(),
            this->positions.end(),
            [&facility, frequency, &type](
                const std::pair<std::string, const std::shared_ptr<ControllerPosition>&>& position) -> bool {
                // Frequency matching is done to 4dp, because floating points.
                return fabs(frequency - position.second->GetFrequency()) < FREQUENCY_MATCH_DELTA &&
                       position.second->GetUnit() == facility && position.second->GetType() == type;
            });

        return position == this->positions.cend() ? nullptr : position->second;
    }

    /*
        Returns the number of elements in the collection.
    */
    auto ControllerPositionCollection::GetSize() const -> size_t
    {
        return this->positions.size();
    }
} // namespace UKControllerPlugin::Controller
