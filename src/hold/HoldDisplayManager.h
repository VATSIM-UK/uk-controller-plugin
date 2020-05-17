#pragma once
#include "euroscope/AsrEventHandlerInterface.h"
#include "hold/HoldDisplay.h"

namespace UKControllerPlugin {
    namespace Hold {
        class HoldManager;
        class HoldDisplayFactory;
    }  // namespace Hold
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            A class that manages hold display classes for each of the ASRs.
        */
        class HoldDisplayManager : public UKControllerPlugin::Euroscope::AsrEventHandlerInterface
        {
            public:
                HoldDisplayManager(
                    const UKControllerPlugin::Hold::HoldManager & holdManager,
                    const UKControllerPlugin::Hold::HoldDisplayFactory & displayFactory
                );
                size_t CountDisplays(void) const;
                unsigned int GetCurrentProfile(void) const;
                const UKControllerPlugin::Hold::HoldDisplay & GetDisplay(std::string fix) const;
                void LoadProfile(unsigned int profileId);

                // Inherited via AsrEventHandlerInterface
                void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;
                void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;

                // Define types for iteration
                typedef std::set<std::unique_ptr<UKControllerPlugin::Hold::HoldDisplay>> HoldDisplays;
                typedef HoldDisplays::const_iterator const_iterator;
                const_iterator cbegin() const { return displays.cbegin(); }
                const_iterator cend() const { return displays.cend(); }

            private:

                void SaveDisplaysToAsr(void) const;

                // Manages holds
                const UKControllerPlugin::Hold::HoldManager & holdManager;

                // Creates displays
                const UKControllerPlugin::Hold::HoldDisplayFactory & displayFactory;

                // The hold displays
                std::set<std::unique_ptr<UKControllerPlugin::Hold::HoldDisplay>> displays;

                // Place to find settings
                UKControllerPlugin::Euroscope::UserSetting * userSetting;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
