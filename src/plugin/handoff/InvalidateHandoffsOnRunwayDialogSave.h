#pragma once
#include "euroscope/RunwayDialogAwareInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Handoff {

    class DepartureHandoffResolver;

    class InvalidateHandoffsOnRunwayDialogSave : public Euroscope::RunwayDialogAwareInterface
    {
        public:
        InvalidateHandoffsOnRunwayDialogSave(
            std::shared_ptr<DepartureHandoffResolver> resolver, Euroscope::EuroscopePluginLoopbackInterface& plugin);
        void RunwayDialogSaved() override;

        private:
        // The resolver
        const std::shared_ptr<DepartureHandoffResolver> resolver;

        // The plugin
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
    };
} // namespace UKControllerPlugin::Handoff
