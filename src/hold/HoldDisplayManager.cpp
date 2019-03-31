#include "pch/stdafx.h"
#include "hold/HoldDisplayManager.h"
#include "hold/HoldProfile.h"
#include "hold/HoldProfileManager.h"
#include "hold/HoldManager.h"
#include "hold/HoldDisplayFactory.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin {
    namespace Hold {

        HoldDisplayManager::HoldDisplayManager(
            const HoldProfileManager & profileManager,
            const HoldManager & holdManager,
            const HoldDisplayFactory & displayFactory
        ) : profileManager(profileManager), holdManager(holdManager), displayFactory(displayFactory)
        {

        }

        void HoldDisplayManager::AsrLoadedEvent(UserSetting & userSetting)
        {
            this->userSetting = &userSetting;
            this->profileId = this->userSetting->GetUnsignedIntegerEntry(
                this->selectedProfileAsrKey,
                this->noProfileSelectedId
            );

            if (this->profileId == this->noProfileSelectedId) {
                return;
            }

            // Load the profile
            this->LoadProfile(this->profileId);
        }

        void HoldDisplayManager::AsrClosingEvent(UserSetting & userSetting)
        {
            // Save the selected profile
            this->userSetting->Save(
                this->selectedProfileAsrKey,
                this->selectedProfileAsrDescription,
                this->profileId
            );

            // Save the profiles
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
                HoldProfile profile = this->profileManager.GetProfile(this->profileId);
                if (profile == this->profileManager.invalidProfile) {
                    return;
                }
                (*it)->SaveDataToAsr(*this->userSetting, this->profileId, profile.name);
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
            Load the given hold profile
        */
        void HoldDisplayManager::LoadProfile(unsigned int profileId)
        {
            this->profileId = profileId;
            HoldProfile selected = this->profileManager.GetProfile(profileId);
            if (selected == this->profileManager.invalidProfile) {
                LogWarning("Tried to load invalid hold profile");
                return;
            }

            // Save the current displays ASR data and clear the displays
            this->SaveDisplaysToAsr();
            this->displays.clear();

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
