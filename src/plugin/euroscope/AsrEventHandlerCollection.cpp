#include "pch/stdafx.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/AsrEventHandlerInterface.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Euroscope::AsrEventHandlerInterface;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            Called when the ASR first opens and has data ready to be processed. Used to notify implementing classes that they
            can start to configure themselves.
        */
        void AsrEventHandlerCollection::AsrLoadedEvent(UserSetting & userSetting) const
        {
            for (
                std::set<std::shared_ptr<AsrEventHandlerInterface>>::const_iterator it = this->allHandlers.cbegin();
                it != this->allHandlers.cend();
                ++it
            ) {
                (*it)->AsrLoadedEvent(userSetting);
            }
        }

        /*
            Called when the ASR is about to close and therefore the implementing
            classes need to save their data now or have it lost.
        */
        void AsrEventHandlerCollection::AsrClosingEvent(UserSetting & userSetting) const
        {
            for (
                std::set<std::shared_ptr<AsrEventHandlerInterface>>::const_iterator it = this->allHandlers.cbegin();
                it != this->allHandlers.cend();
                ++it
            ) {
                (*it)->AsrClosingEvent(userSetting);
            }
        }

        int AsrEventHandlerCollection::CountHandlers(void) const
        {
            return this->allHandlers.size();
        }

        /*
            Called to register a handler with this collection.
        */
        void AsrEventHandlerCollection::RegisterHandler(std::shared_ptr<AsrEventHandlerInterface> handler)
        {
            this->allHandlers.insert(handler);
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
