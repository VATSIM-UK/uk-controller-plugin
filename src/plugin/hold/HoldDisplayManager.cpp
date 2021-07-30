#include "pch/pch.h"
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
        ) : holdManager(holdManager), displayFactory(displayFactory), userSetting(nullptr)
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
            this->LoadSelectedHolds(this->userSetting->GetStringListEntry(selectedHoldsAsrKey, {}));
        }

        /*
            Save data to the ASR
        */
        void HoldDisplayManager::AsrClosingEvent(UserSetting & userSetting)
        {
            this->userSetting->Save(
                this->selectedHoldsAsrKey,
                this->selectedHoldsAsrDescription,
                this->selectedHolds
            );
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
            Return a display by hold fix
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

        std::vector<std::string> HoldDisplayManager::GetSelectedHolds(void) const
        {
            return this->selectedHolds;
        }

        /*
            Load the given hold profile
        */
        void HoldDisplayManager::LoadSelectedHolds(std::vector<std::string> holds)
        {
            this->SaveDisplaysToAsr();
            this->displays.clear();

            // Setup new profile
            this->selectedHolds = holds;
            for (std::vector<std::string>::const_iterator holdIt = holds.cbegin();
                holdIt != holds.cend();
                ++holdIt
            ) {
                std::unique_ptr<HoldDisplay> display = this->displayFactory.Create(*holdIt);
                if (display == nullptr) {
                    LogWarning("Failed to create hold display for " + *holdIt);
                    continue;
                }

                display->LoadDataFromAsr(*this->userSetting);
                this->displays.insert(std::move(display));
            }
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
