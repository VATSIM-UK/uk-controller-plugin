#pragma once
#include "euroscope/EuroScopeCControllerInterface.h"

namespace UKControllerPlugin::Euroscope {

    /*
        A class wrapping the Euroscope CController class.
    */
    class EuroScopeCControllerWrapper : public EuroScopeCControllerInterface
    {
        public:
        EuroScopeCControllerWrapper(EuroScopePlugIn::CController originalData, bool isCurrentUser);
        [[nodiscard]] auto IsVatsimRecognisedController() const -> bool override;
        [[nodiscard]] auto GetCallsign() const -> const std::string override;
        [[nodiscard]] auto GetControllerName() const -> const std::string override;
        [[nodiscard]] auto GetFrequency() const -> double override;
        [[nodiscard]] auto HasActiveFrequency() const -> bool override;
        [[nodiscard]] auto IsCurrentUser() const -> bool override;

        private:
        // The original data
        EuroScopePlugIn::CController originalData;

        // Whether or not this controller is the current user
        bool isCurrentUser;
    };
} // namespace UKControllerPlugin::Euroscope
