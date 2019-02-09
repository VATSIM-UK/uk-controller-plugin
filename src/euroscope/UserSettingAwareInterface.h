#pragma once
#include "euroscope/UserSetting.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            An interface to be implemented when the class
            wishes to be notified that the main user settings have been updated.
        */
        class UserSettingAwareInterface
        {
            public:

                /*
                    Should be called whenever user settings have been updated.
                */
                virtual void UserSettingsUpdated(UKControllerPlugin::Euroscope::UserSetting & userSettings) = 0;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
