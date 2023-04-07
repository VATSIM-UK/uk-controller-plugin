#pragma once
#include "euroscope/PluginSettingsProviderInterface.h"

namespace UKControllerPlugin {
    namespace Airfield {
        class AirfieldCollection;
    } // namespace Airfield
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Approach {
    class ApproachSequencerOptions;

    /**
     * Loads the approach sequencer options from EuroScope configuration.
     */
    class ApproachSequencerOptionsLoader : public Euroscope::PluginSettingsProviderInterface
    {
        public:
        ApproachSequencerOptionsLoader(
            ApproachSequencerOptions& options, const Airfield::AirfieldCollection& airfields);
        void LoadSettings(Euroscope::UserSetting& settings) override;
        void SaveSettings(Euroscope::UserSetting& settings) override;

        private:
        // A value to use as the default so we know if a setting is present.
        inline static const unsigned int SETTING_NOT_PRESENT = 99999;

        [[nodiscard]] static auto MakeModeKey(const std::string& airfield) -> std::string;
        [[nodiscard]] static auto MakeTargetKey(const std::string& airfield) -> std::string;
        [[nodiscard]] static auto MakeSeparationKey(const std::string& airfield) -> std::string;
        [[nodiscard]] static auto MakeModeDescription(const std::string& airfield) -> std::string;
        [[nodiscard]] static auto MakeTargetDescription(const std::string& airfield) -> std::string;
        [[nodiscard]] static auto MakeSeparationDescription(const std::string& airfield) -> std::string;
        [[nodiscard]] static auto MakeKeyBase(const std::string& airfield, const std::string& item) -> std::string;
        [[nodiscard]] static auto MakeDescriptionBase(const std::string& airfield, const std::string& item)
            -> std::string;

        // Where options are stored whilst plugin is running
        ApproachSequencerOptions& options;

        // For loading settings
        const Airfield::AirfieldCollection& airfields;
    };
} // namespace UKControllerPlugin::Approach
