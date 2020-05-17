#include "pch/stdafx.h"
#include "hold/HoldDisplayManager.h"
#include "hold/HoldManager.h"
#include "hold/HoldDisplayFactory.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin {
    namespace Hold {

        HoldDisplayManager::HoldDisplayManager(
            const HoldManager & holdManager,
            const HoldDisplayFactory & displayFactory
        ) : holdManager(holdManager), displayFactory(displayFactory)
        {

        }

        /*
            Return the number of displays
        */
        size_t HoldDisplayManager::CountDisplays(void) const
        {
            return this->displays.size();
        }

        /*
            Load data from the ASR
        */
        void HoldDisplayManager::AsrLoadedEvent(UserSetting & userSetting)
        {
            this->userSetting = &userSetting;

            // Load the profile
            this->LoadProfile(this->profileId);
        }

        /*
            Save data to the ASR
        */
        void HoldDisplayManager::AsrClosingEvent(UserSetting & userSetting)
        {
            this->SaveDisplaysToAsr();
        }

        /*
            Save all the loaded displays data to the ASR
        */
        void HoldDisplayManager::SaveDisplaysToAsr(void) const
        {
            for (
                std::set<std::unique_ptr<HoldDisplay>>::const_iterator it = this->displays.cbegin();
                it != this->displays.cend();
                ++it
            ) {
                (*it)->SaveDataToAsr(*this->userSetting);
            }
        }

        /*
            Return the current profile ID
        */
        unsigned int HoldDisplayManager::GetCurrentProfile(void) const
        {
            return this->profileId;
        }

        /*
            Return a display by hold ID, mainly for testing
        */
        const UKControllerPlugin::Hold::HoldDisplay & HoldDisplayManager::GetDisplay(std::string fix) const
        {
            auto display = std::find_if(
                this->displays.cbegin(),
                this->displays.cend(),
                [fix](const std::unique_ptr<HoldDisplay> & display) -> bool {
                    return display->navaid.identifier == fix;
                }
            );

            return **display;
        }

        /*
            Load the given hold profile
        */
        void HoldDisplayManager::LoadProfile(unsigned int profileId)
        {
            HoldProfile selected = this->profileManager.GetProfile(profileId);
            if (selected == this->profileManager.invalidProfile) {
                LogWarning("Tried to load invalid hold profile");
                return;
            }

            // Save the current displays ASR data and clear the displays
            this->SaveDisplaysToAsr();
            this->displays.clear();

            // Setup new profile
            this->profileId = profileId;
            for (std::set<unsigned int>::const_iterator holdIt = selected.holds.cbegin();
                holdIt != selected.holds.cend();
                ++holdIt
            ) {
                std::unique_ptr<HoldDisplay> display = this->displayFactory.Create(*holdIt);
                if (display == nullptr) {
                    LogWarning("Failed to create hold display for " + std::to_string(*holdIt));
                    continue;
                }

                display->LoadDataFromAsr(*this->userSetting, this->profileId);
                this->displays.insert(std::move(display));
            }
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
