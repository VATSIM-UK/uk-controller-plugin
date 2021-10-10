#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Ownership {
        class AirfieldServiceProviderCollection;
    } // namespace Ownership
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachOptions;
    class MissedApproach;

    /**
     * Class responsible for playing the missed approach audio alert
     * of a missed approach.
     */
    class MissedApproachAudioAlert
    {
        public:
        MissedApproachAudioAlert(
            std::shared_ptr<const MissedApproachOptions> options,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            const Ownership::AirfieldServiceProviderCollection& airfieldServiceProvisions,
            Windows::WinApiInterface& winApi);
        void Play(const std::shared_ptr<class MissedApproach>& missedApproach);

        private:
        [[nodiscard]] auto ShouldPlay(const std::shared_ptr<class MissedApproach>& missedApproach) const -> bool;
        
        // Options for missed approaches
        std::shared_ptr<const MissedApproachOptions> options;

        // Plugin for retrieving flightplans
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // Who's providing what services at airfields
        const Ownership::AirfieldServiceProviderCollection& airfieldServiceProvisions;

        // For playing sounds
        Windows::WinApiInterface& winApi;
    };
} // namespace UKControllerPlugin::MissedApproach
