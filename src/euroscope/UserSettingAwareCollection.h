#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Euroscope {
        class UserSetting;
        class UserSettingAwareInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            A collection of objects that need to be notified when a settings file is loaded or
            about to be closed.
        */
        class UserSettingAwareCollection
        {
            public:
                int Count(void) const;
                void RegisterHandler(
                    std::shared_ptr<UKControllerPlugin::Euroscope::UserSettingAwareInterface> handler
                );
                void UserSettingsUpdateEvent(UKControllerPlugin::Euroscope::UserSetting & asrStore) const;

            private:
                std::set<std::shared_ptr<UKControllerPlugin::Euroscope::UserSettingAwareInterface>> allHandlers;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
