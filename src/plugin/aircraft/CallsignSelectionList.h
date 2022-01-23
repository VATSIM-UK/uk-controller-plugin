#pragma once
#include "CallsignSelectionListInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Aircraft {
    class CallsignSelectionProviderInterface;

    /**
     * Implements a callsign selection list.
     */
    class CallsignSelectionList : public CallsignSelectionListInterface
    {
        public:
        CallsignSelectionList(
            std::shared_ptr<CallsignSelectionProviderInterface> callsignProvider,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            int callbackFunctionId);
        ~CallsignSelectionList() = default;
        void TriggerList(const POINT& location) override;
        void CallsignSelected(const std::string& callsign);

        private:
        // Provides the callsigns
        std::shared_ptr<CallsignSelectionProviderInterface> callsignProvider;

        // The plugin for triggering lists
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // The callback function
        int callbackFunctionId;
    };
} // namespace UKControllerPlugin::Aircraft
