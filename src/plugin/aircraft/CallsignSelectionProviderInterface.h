#pragma once

namespace UKControllerPlugin::Aircraft {
    /**
     * An interface for classes that provide the callsign
     * selection list with callsigns and act when one
     * is selected.
     */
    class CallsignSelectionProviderInterface
    {
        public:
        virtual ~CallsignSelectionProviderInterface() = default;
        [[nodiscard]] virtual auto GetCallsigns() -> std::set<std::string> = 0;
        virtual void CallsignSelected(const std::string& callsign) = 0;
    };
} // namespace UKControllerPlugin::Aircraft
