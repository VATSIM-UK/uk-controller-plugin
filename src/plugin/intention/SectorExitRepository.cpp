#include "pch/pch.h"
#include "intention/SectorExitRepository.h"
#include "intention/SectorExitPointEtrat.h"
#include "intention/SectorExitPointLelna.h"
#include "intention/SectorExitPointVeule.h"

namespace UKControllerPlugin {
    namespace IntentionCode {

        /*
            Add all the SEPs to the map.
        */
        SectorExitRepository::SectorExitRepository(std::map<std::string, std::unique_ptr<SectorExitPoint>> exitMap)
            : exitMap(std::move(exitMap))
        {

        }

        /*
            Returns true if the named sector exit point is known to the class.
        */
        bool SectorExitRepository::HasSectorExitPoint(std::string point) const
        {
            return this->exitMap.count(point) == 1;
        }

        /*
            Returns a shared autopointer to a given SectorExitPoint, or to NULL if not found.
        */
        const SectorExitPoint & SectorExitRepository::GetSectorExitPoint(std::string point) const
        {
            if (!this->HasSectorExitPoint(point)) {
                throw new std::invalid_argument("Exit point not found");
            }

            return *this->exitMap.at(point);
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
