#pragma once
#include "euroscope/EuroScopeCControllerInterface.h"

namespace EuroScopePlugIn {
    class CController;
}  // namespace EuroScopePlugIn

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            A class wrapping the Euroscope CController class.
        */
        class EuroScopeCControllerWrapper : public EuroScopeCControllerInterface
        {
            public:
                EuroScopeCControllerWrapper(EuroScopePlugIn::CController originalData, bool isCurrentUser);
                bool IsVatsimRecognisedController(void) const override;
                const std::string GetCallsign(void) const;
                const std::string GetControllerName(void) const;
                const double GetFrequency(void) const;
                const bool HasActiveFrequency(void) const;
                const bool IsCurrentUser(void) const;

            private:

                // The original data
                EuroScopePlugIn::CController originalData;

                // Whether or not this controller is the current user
                bool isCurrentUser;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
