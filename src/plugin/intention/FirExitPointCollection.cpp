#include "FirExitPoint.h"
#include "FirExitPointCollection.h"

namespace UKControllerPlugin::IntentionCode {

    void FirExitPointCollection::Add(const std::shared_ptr<FirExitPoint>& point)
    {
        if (points.contains(point->Identifier())) {
            LogWarning("Duplicate FIR exit point added");
            return;
        }

        points[point->Identifier()] = point;
    }

    auto FirExitPointCollection::CountPoints() const -> size_t
    {
        return points.size();
    }

    auto FirExitPointCollection::PointByIdentifier(const std::string& identifier) const -> std::shared_ptr<FirExitPoint>
    {
        return points.contains(identifier) ? points.at(identifier) : nullptr;
    }
} // namespace UKControllerPlugin::IntentionCode
