#include "intention/SectorExitPoint.h"
#include "intention/SectorExitRepository.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;

namespace UKControllerPlugin {
    namespace IntentionCode {

        SectorExitPoint::SectorExitPoint(std::string name, std::string intentionCode, unsigned int outDirection)
        {
            this->name = name;
            this->intentionCode = intentionCode;
            this->outDirection = outDirection;
        }

        /*
            Get the name of the sector exit FIX.
        */
        std::string SectorExitPoint::GetName(void) const
        {
            return this->name;
        }

        /*
            Get the intention code that corresponds to the sector exit FIX.
        */
        std::string SectorExitPoint::GetIntentionCode(
            EuroscopeExtractedRouteInterface & route,
            int foundPointIndex,
            int cruiseLevel
        ) const {
            return this->intentionCode;
        }

        /*
            Get the direction that corresponds to the sector exit fix.
        */
        unsigned int SectorExitPoint::GetOutDirection(void) const
        {
            return this->outDirection;
        }


        /*
            Checks that the direction of travel is appropriate for this sector exit point.
            This function is used to stop multi-directional airways from using the sector
            entry point rather than the sector  exit.
        */
        bool SectorExitPoint::IsCorrectOutDirection(double directionOfTravel) const
        {
            // Invalid heading, return false.
            if (directionOfTravel < 0.0 || directionOfTravel > 360.0) {
                return false;
            }

            bool result = false;

            if ((this->outDirection & this->outNorth) == this->outNorth) {
                result = result || (directionOfTravel <= 55.0 || directionOfTravel >= 305.0);
            }

            if ((this->outDirection & this->outEast) == this->outEast) {
                result = result || (directionOfTravel <= 145.0 && directionOfTravel >= 35.0);
            }

            if ((this->outDirection & this->outSouth) == this->outSouth) {
                result = result || (directionOfTravel <= 235.0 && directionOfTravel >= 125.0);
            }

            if ((this->outDirection & this->outWest) == this->outWest) {
                result = result || (directionOfTravel <= 325.0 && directionOfTravel >= 215.0);
            }

            if ((this->outDirection & this->outSouthEast) == this->outSouthEast) {
                result = result || (directionOfTravel <= 180.0 && directionOfTravel >= 55.0);
            }

            if ((this->outDirection & this->outSouthWest) == this->outSouthWest) {
                result = result || (directionOfTravel >= 180.0 && directionOfTravel <= 305.0);
            }

            if ((this->outDirection & this->outNorthWest) == this->outNorthWest) {
                result = result || (directionOfTravel <= 360.0 && directionOfTravel >= 260.0);
            }

            if ((this->outDirection & this->outNorthEast) == this->outNorthEast) {
                result = result || (directionOfTravel >= 0.00 && directionOfTravel <= 125.0);
            }
            // Default return no.
            return result;
        }

        bool SectorExitPoint::AppliesToController(std::string callsign) const
        {
            return true;
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
