#include "euroscope/EuroScopeCControllerWrapper.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        EuroScopeCControllerWrapper::EuroScopeCControllerWrapper(
            EuroScopePlugIn::CController originalData, bool isCurrentUser)
            : originalData(originalData)
        {
            this->isCurrentUser = isCurrentUser;
        }

        bool EuroScopeCControllerWrapper::IsVatsimRecognisedController(void) const
        {
            return this->originalData.IsController();
        }

        const std::string EuroScopeCControllerWrapper::GetCallsign(void) const
        {
            return this->originalData.GetCallsign();
        }

        const std::string EuroScopeCControllerWrapper::GetControllerName(void) const
        {
            return this->originalData.GetFullName();
        }

        double EuroScopeCControllerWrapper::GetFrequency(void) const
        {
            return this->originalData.GetPrimaryFrequency();
        }

        bool EuroScopeCControllerWrapper::HasActiveFrequency(void) const
        {
            return this->originalData.GetPrimaryFrequency() < 190.000;
        }

        bool EuroScopeCControllerWrapper::IsCurrentUser(void) const
        {
            return this->isCurrentUser;
        }
    } // namespace Euroscope
} // namespace UKControllerPlugin
