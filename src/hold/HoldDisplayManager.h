#pragma once
#include "euroscope/AsrEventHandlerInterface.h"
#include "hold/HoldDisplay.h"

namespace UKControllerPlugin {
    namespace Hold {
        class HoldProfileManager;
        class HoldManager;
        class HoldDisplayFactory;
    }  // namespace Hold
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            A class that manages hold display classes based on the selected
            profile. Separates loading the displays from actually rendering them. This class
            is instantiated per-ASR
        */
        class HoldDisplayManager : public UKControllerPlugin::Euroscope::AsrEventHandlerInterface
        {
            public:
                HoldDisplayManager(
                    const UKControllerPlugin::Hold::HoldProfileManager & profileManager,
                    const UKControllerPlugin::Hold::HoldManager & holdManager,
                    const UKControllerPlugin::Hold::HoldDisplayFactory & displayFactory
                );
                size_t CountDisplays(void) const;
                unsigned int GetCurrentProfile(void) const;
                const UKControllerPlugin::Hold::HoldDisplay & GetDisplay(unsigned int holdId) const;
                void LoadProfile(unsigned int profileId);

                // Inherited via AsrEventHandlerInterface
                void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;
                void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;

                // Define types for iteration
                typedef std::set<std::unique_ptr<UKControllerPlugin::Hold::HoldDisplay>> HoldDisplays;
                typedef HoldDisplays::const_iterator const_iterator;
                const_iterator cbegin() const { return displays.cbegin(); }
                const_iterator cend() const { return displays.cend(); }

                // The profile id if no profile has been selected
                const unsigned int noProfileSelectedId = 0;

                // The key in the ASR for the selected hold profile
                const std::string selectedProfileAsrKey = "selectedHoldProfile";

                // The description for the ASR item
                const std::string selectedProfileAsrDescription = "Selected Hold Profile";

            private:

                void SaveDisplaysToAsr(void) const;
                
                // Manages profiles
                const UKControllerPlugin::Hold::HoldProfileManager & profileManager;

                // Manages holds
                const UKControllerPlugin::Hold::HoldManager & holdManager;
                
                // Creates displays
                const UKControllerPlugin::Hold::HoldDisplayFactory & displayFactory;

                // The hold displays
                std::set<std::unique_ptr<UKControllerPlugin::Hold::HoldDisplay>> displays;

                // Place to find settings
                UKControllerPlugin::Euroscope::UserSetting * userSetting;

                // The currently selected profile id
                unsigned int profileId = 0;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
