#pragma once
#include "euroscope/AsrEventHandlerInterface.h"

namespace UKControllerPlugin::Hold {
    class HoldDisplay;
    class HoldDisplayFactory;

    /*
        A class that manages hold display classes for each of the ASRs.
    */
    class HoldDisplayManager : public UKControllerPlugin::Euroscope::AsrEventHandlerInterface
    {
        public:
        HoldDisplayManager(const UKControllerPlugin::Hold::HoldDisplayFactory& displayFactory);
        ~HoldDisplayManager();
        [[nodiscard]] auto CountDisplays() const -> size_t;
        [[nodiscard]] auto GetDisplay(const std::string& fix) const -> const UKControllerPlugin::Hold::HoldDisplay&;
        [[nodiscard]] auto GetSelectedHolds() const -> std::vector<std::string>;
        void LoadSelectedHolds(const std::vector<std::string>& holds);

        // Inherited via AsrEventHandlerInterface
        void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting& userSetting) override;
        void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting& userSetting) override;

        // Define types for iteration
        using HoldDisplays = std::set<std::unique_ptr<UKControllerPlugin::Hold::HoldDisplay>>;
        using const_iterator = HoldDisplays::const_iterator;
        [[nodiscard]] auto cbegin() const -> const_iterator
        {
            return displays.cbegin();
        }
        [[nodiscard]] auto cend() const -> const_iterator
        {
            return displays.cend();
        }

        private:
        void SaveDisplaysToAsr() const;

        // Creates displays
        const UKControllerPlugin::Hold::HoldDisplayFactory& displayFactory;

        // The hold displays
        std::set<std::unique_ptr<UKControllerPlugin::Hold::HoldDisplay>> displays;

        // Place to find settings
        UKControllerPlugin::Euroscope::UserSetting* userSetting;

        // The currently selected holds
        std::vector<std::string> selectedHolds;

        // ASR Data
        const std::string selectedHoldsAsrKey = "selectedHolds";

        const std::string selectedHoldsAsrDescription = "Selected Holds";
    };
} // namespace UKControllerPlugin::Hold
