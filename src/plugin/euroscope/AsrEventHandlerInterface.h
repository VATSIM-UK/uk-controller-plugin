#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Euroscope {
        class UserSetting;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
// END


namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            Interface for classes that need to be notified when settings files
            are loaded or needs to be saved.
        */
        class AsrEventHandlerInterface
        {
            public:
                /*
                    Called when the settings file first opens and has data ready to be processed. Used to notify implementing classes that they
                    can start to configure themselves.
                */
                virtual void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) = 0;

                /*
                    Called when the settings file is about to close and therefore the implementing classes need to save their data now or have
                    it lost.
                */
                virtual void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) = 0;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
