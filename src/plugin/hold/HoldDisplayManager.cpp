#include "HoldDisplay.h"
#include "HoldDisplayFactory.h"
#include "HoldDisplayManager.h"
#include "euroscope/UserSetting.h"
#include "navaids/Navaid.h"

using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin::Hold {

    HoldDisplayManager::HoldDisplayManager(const HoldDisplayFactory& displayFactory)
        : displayFactory(displayFactory), userSetting(nullptr)
    {
    }

    HoldDisplayManager::~HoldDisplayManager() = default;

    /*
        Return the number of displays
    */
    auto HoldDisplayManager::CountDisplays() const -> size_t
    {
        return this->displays.size();
    }

    /*
        Load data from the ASR
    */
    void HoldDisplayManager::AsrLoadedEvent(UserSetting& userSetting)
    {
        this->userSetting = &userSetting;

        // Load the profile
        this->LoadSelectedHolds(this->userSetting->GetStringListEntry(selectedHoldsAsrKey, {}));
    }

    /*
        Save data to the ASR
    */
    void HoldDisplayManager::AsrClosingEvent(UserSetting& userSetting)
    {
        this->userSetting->Save(this->selectedHoldsAsrKey, this->selectedHoldsAsrDescription, this->selectedHolds);
        this->SaveDisplaysToAsr();
    }

    /*
        Save all the loaded displays data to the ASR
    */
    void HoldDisplayManager::SaveDisplaysToAsr() const
    {
        for (auto it = this->displays.cbegin(); it != this->displays.cend(); ++it) {
            (*it)->SaveDataToAsr(*this->userSetting);
        }
    }

    /*
        Return a display by hold fix
    */
    auto HoldDisplayManager::GetDisplay(const std::string& fix) const -> const UKControllerPlugin::Hold::HoldDisplay&
    {
        auto display = std::find_if(
            this->displays.cbegin(), this->displays.cend(), [fix](const std::unique_ptr<HoldDisplay>& display) -> bool {
                return display->navaid.identifier == fix;
            });

        return **display;
    }

    auto HoldDisplayManager::GetSelectedHolds() const -> std::vector<std::string>
    {
        return this->selectedHolds;
    }

    /*
        Load the given hold profile
    */
    void HoldDisplayManager::LoadSelectedHolds(const std::vector<std::string>& holds)
    {
        this->SaveDisplaysToAsr();
        this->displays.clear();

        // Setup new profile
        this->selectedHolds = holds;
        for (auto holdIt = holds.cbegin(); holdIt != holds.cend(); ++holdIt) {
            std::unique_ptr<HoldDisplay> display = this->displayFactory.Create(*holdIt);
            if (display == nullptr) {
                LogWarning("Failed to create hold display for " + *holdIt);
                continue;
            }

            display->LoadDataFromAsr(*this->userSetting);
            this->displays.insert(std::move(display));
        }
    }
} // namespace UKControllerPlugin::Hold
