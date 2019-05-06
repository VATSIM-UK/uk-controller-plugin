#include "pch/stdafx.h"
#include "euroscope/EuroScopeCControllerWrapper.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        EuroScopeCControllerWrapper::EuroScopeCControllerWrapper(
            EuroScopePlugIn::CController originalData,
            bool isCurrentUser
        )
            : originalData(originalData)
        {
            this->isCurrentUser = isCurrentUser;
        }

        const std::string EuroScopeCControllerWrapper::GetCallsign(void) const
        {
            return this->originalData.GetCallsign();
        }

        const std::string EuroScopeCControllerWrapper::GetControllerName(void) const
        {
            return this->originalData.GetFullName();
        }

        const double EuroScopeCControllerWrapper::GetFrequency(void) const
        {
            return this->originalData.GetPrimaryFrequency();
        }

        const bool EuroScopeCControllerWrapper::HasActiveFrequency(void) const
        {
            return this->originalData.GetPrimaryFrequency() < 190.000;
        }

        const bool EuroScopeCControllerWrapper::IsCurrentUser(void) const
        {
            return this->isCurrentUser;
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
