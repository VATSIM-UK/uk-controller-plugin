#include "AirfieldApproachOptions.h"
#include "ApproachSequencerOptions.h"
#include "ApproachSequencerOptionsLoader.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPlugin::Approach {

    ApproachSequencerOptionsLoader::ApproachSequencerOptionsLoader(
        ApproachSequencerOptions& options, const Airfield::AirfieldCollection& airfields)
        : options(options), airfields(airfields)
    {
    }

    void ApproachSequencerOptionsLoader::LoadSettings(Euroscope::UserSetting& settings)
    {
        airfields.ForEach([&settings, this](const Airfield::AirfieldModel& airfield) {
            auto testValue = settings.GetUnsignedIntegerEntry(MakeModeKey(airfield.Icao()), SETTING_NOT_PRESENT);
            if (testValue == SETTING_NOT_PRESENT) {
                return;
            }

            auto options = std::make_shared<AirfieldApproachOptions>();

            // Load the mode
            auto mode = static_cast<ApproachSequencingMode>(settings.GetUnsignedIntegerEntry(
                MakeModeKey(airfield.Icao()), static_cast<unsigned int>(options->defaultMode)));
            if (mode != ApproachSequencingMode::WakeTurbulence && mode != ApproachSequencingMode::MinimumDistance) {
                mode = ApproachSequencingMode::WakeTurbulence;
            }

            options->defaultMode = mode;

            // Load the target distance
            options->targetDistance = settings.GetFloatEntry(MakeTargetKey(airfield.Icao()), options->targetDistance);

            // Load the minimum separation requirement
            options->minimumSeparationRequirement =
                settings.GetFloatEntry(MakeSeparationKey(airfield.Icao()), options->minimumSeparationRequirement);

            this->options.Set(airfield.Icao(), options);
        });
    }

    void ApproachSequencerOptionsLoader::SaveSettings(Euroscope::UserSetting& settings)
    {
        options.ForEach([&settings](const std::string& airfield, const AirfieldApproachOptions& options) {
            settings.Save(
                MakeModeKey(airfield), MakeModeDescription(airfield), static_cast<unsigned int>(options.defaultMode));
            settings.Save(MakeTargetKey(airfield), MakeTargetDescription(airfield), options.targetDistance);
            settings.Save(
                MakeSeparationKey(airfield), MakeSeparationDescription(airfield), options.minimumSeparationRequirement);
        });
    }

    auto ApproachSequencerOptionsLoader::MakeModeKey(const std::string& airfield) -> std::string
    {
        return MakeKeyBase(airfield, "Mode");
    }

    auto ApproachSequencerOptionsLoader::MakeTargetKey(const std::string& airfield) -> std::string
    {
        return MakeKeyBase(airfield, "Target");
    }

    auto ApproachSequencerOptionsLoader::MakeSeparationKey(const std::string& airfield) -> std::string
    {
        return MakeKeyBase(airfield, "Sep");
    }

    auto ApproachSequencerOptionsLoader::MakeModeDescription(const std::string& airfield) -> std::string
    {
        return MakeDescriptionBase(airfield, "Default Mode");
    }

    auto ApproachSequencerOptionsLoader::MakeTargetDescription(const std::string& airfield) -> std::string
    {
        return MakeDescriptionBase(airfield, "Default Target Distance");
    }

    auto ApproachSequencerOptionsLoader::MakeSeparationDescription(const std::string& airfield) -> std::string
    {
        return MakeDescriptionBase(airfield, "Default Minimum Separation");
    }

    auto ApproachSequencerOptionsLoader::MakeKeyBase(const std::string& airfield, const std::string& item)
        -> std::string
    {
        return "approachSequencer" + item + airfield;
    }

    auto ApproachSequencerOptionsLoader::MakeDescriptionBase(const std::string& airfield, const std::string& item)
        -> std::string
    {
        return airfield + " Approach Sequencer " + item;
    }
} // namespace UKControllerPlugin::Approach
