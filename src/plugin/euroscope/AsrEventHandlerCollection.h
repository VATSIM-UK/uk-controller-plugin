#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Euroscope {
        class UserSetting;
        class AsrEventHandlerInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            A collection of objects that need to be notified when a settings file is loaded or
            about to be closed.
        */
        class AsrEventHandlerCollection
        {
            public:
            void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting& asrStore) const;
            void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting& asrStore) const;
            int CountHandlers(void) const;
            void RegisterHandler(std::shared_ptr<UKControllerPlugin::Euroscope::AsrEventHandlerInterface> handler);

            private:
            std::set<std::shared_ptr<UKControllerPlugin::Euroscope::AsrEventHandlerInterface>> allHandlers;
        };
    } // namespace Euroscope
} // namespace UKControllerPlugin
