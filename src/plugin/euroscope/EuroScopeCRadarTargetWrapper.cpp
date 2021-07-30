#include "pch/stdafx.h"
#include "euroscope/EuroScopeCRadarTargetWrapper.h"

using EuroScopePlugIn::CRadarTarget;

namespace UKControllerPlugin {
    namespace Euroscope {

        EuroScopeCRadarTargetWrapper::EuroScopeCRadarTargetWrapper(EuroScopePlugIn::CRadarTarget originalData)
        {
            this->originalData = originalData;
        }

        const std::string EuroScopeCRadarTargetWrapper::GetCallsign(void) const
        {
            return this->originalData.GetCallsign();
        }

        int EuroScopeCRadarTargetWrapper::GetFlightLevel(void) const
        {
            return this->originalData.GetPosition().GetFlightLevel();
        }

        const EuroScopePlugIn::CPosition EuroScopeCRadarTargetWrapper::GetPosition(void) const
        {
            return this->originalData.GetPosition().GetPosition();
        }

        /*
            Returns the radar targets ground speed. Initially, will use that reported from the pilot
            client. If that's 0, will use what EuroScope estimates.
        */
        const int EuroScopeCRadarTargetWrapper::GetGroundSpeed(void) const
        {
            return this->originalData.GetGS();
        }

        int EuroScopeCRadarTargetWrapper::GetVerticalSpeed(void) const
        {
            return this->originalData.GetVerticalSpeed();
        }

        double EuroScopeCRadarTargetWrapper::GetHeading() const
        {
            return this->originalData.GetTrackHeading();
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
