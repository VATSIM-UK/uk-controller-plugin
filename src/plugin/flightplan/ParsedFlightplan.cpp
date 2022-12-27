#include "FlightplanPoint.h"
#include "ParsedFlightplan.h"

namespace UKControllerPlugin::Flightplan {

    void ParsedFlightplan::AddPoint(const std::shared_ptr<FlightplanPoint>& point)
    {
        if (pointsByIndex.contains(point->Index())) {
            LogError("Tried to add to duplicate parsed flightplan point");
            return;
        }

        pointsByIndex[point->Index()] = point;
        pointsByIdentifier[point->Identifier()].insert(point->Index());
    }

    auto ParsedFlightplan::CountPoints() const -> size_t
    {
        return pointsByIndex.size();
    }

    auto ParsedFlightplan::HasPointByIdentifier(const std::string& identifier) const -> bool
    {
        return pointsByIdentifier.contains(identifier);
    }

    auto ParsedFlightplan::PointByIndex(int index) const -> std::shared_ptr<FlightplanPoint>
    {
        return pointsByIndex.contains(index) ? pointsByIndex.at(index) : nullptr;
    }
} // namespace UKControllerPlugin::Flightplan
