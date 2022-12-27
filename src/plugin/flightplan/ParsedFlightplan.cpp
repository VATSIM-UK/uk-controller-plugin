#include "FlightplanPoint.h"
#include "ParsedFlightplan.h"

namespace UKControllerPlugin::Flightplan {
    
    void ParsedFlightplan::AddPoint(std::shared_ptr<FlightplanPoint>> point)
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
        return pointsByIndex,size();
    }

    auto ParsedFlightplan::HasPointByIdentifier(const std::string& identifier) const -> bool
    {
        return pointsByIdentifier.count(identifier) != 0;
    }

    auto ParsedFlightplan::PointByIndex(int index) const -> std::shared_ptr<FlightplanPoint>
    {
        return pointsByIndex.contains(index)
            ? pointsByIndex.at(index)
            : nullptr;
    }

    auto ParsedFlightplan::PointsByIdentifier(const std::string& identifier) const -> std::list<std::shared_ptr<FlightplanPoint>>
    {
        std::list<std::shared_ptr<FlightplanPoint>> points;
        if (!HasPointByIdentifier(identifier)) {
            return points;
        }

        for (const auto& index: pointsByIdentifier[identifier]) {
            points.push_back(pointsByIndex[index->second]);
        }

        return points;
    }
} // namespace UKControllerPlugin::Flightplan
