#include "pch/stdafx.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "euroscope/UserSettingAwareInterface.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Euroscope::UserSettingAwareInterface;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            Should be called whenever the user settings dialog is saved or the plugin
            loads up.
        */
        void UserSettingAwareCollection::UserSettingsUpdateEvent(UserSetting & userSetting) const
        {
            for (
                std::set<std::shared_ptr<UserSettingAwareInterface>>::const_iterator it = this->allHandlers.cbegin();
                it != this->allHandlers.cend();
                ++it
            ) {
                (*it)->UserSettingsUpdated(userSetting);
            }
        }

        int UserSettingAwareCollection::Count(void) const
        {
            return this->allHandlers.size();
        }

        /*
            Called to register a handler with this collection.
        */
        void UserSettingAwareCollection::RegisterHandler(std::shared_ptr<UserSettingAwareInterface> handler)
        {
            this->allHandlers.insert(handler);
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
