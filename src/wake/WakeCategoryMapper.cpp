#include "pch/stdafx.h"
#include "wake/WakeCategoryMapper.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;

namespace UKControllerPlugin {
    namespace Wake {

        void WakeCategoryMapper::AddCategoryMapping(std::string type, std::string category)
        {
            if (this->categoryMap.count(type) > 0) {
                return;
            }

            this->categoryMap[type] = category;
        }

        /*
            Return count
        */
        int WakeCategoryMapper::Count(void) const
        {
            return this->categoryMap.size();
        }

        std::string WakeCategoryMapper::GetCategoryForAircraftType(
            std::string aircraftType
        ) const
        {
            return this->categoryMap.count(aircraftType) == 1
                ? this->categoryMap.at(aircraftType)
                : this->noCategory;
        }
    }  // namespace Wake
}  // namespace UKControllerPlugin
