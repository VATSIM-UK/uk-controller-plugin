#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        struct CallbackFunction;
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
        class EuroscopePluginLoopbackInterface;
        class EuroscopeRadarLoopbackInterface;
    } // namespace Euroscope
    namespace Tag {
        struct TagFunction;
        struct RadarScreenTagFunction;
    } // namespace Tag
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Plugin {

    /*
        Class to handle Euroscope OnFunctionCall
        events. These events could be raised by CRadarScreen
        and CPlugin derivatives.
    */
    class FunctionCallEventHandler
    {
        public:
        FunctionCallEventHandler();
        ~FunctionCallEventHandler();
        void CallFunction(
            int functionId,
            const std::string& subject,
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            const POINT& mousePos,
            const RECT& area) const;
        void CallFunction(
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int functionId,
            const std::string& subject,
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            const POINT& mousePos,
            const RECT& area) const;
        [[nodiscard]] auto CountCallbacks() const -> size_t;
        [[nodiscard]] auto CountTagFunctions() const -> size_t;
        [[nodiscard]] auto CountRadarScreenTagFunctions() const -> size_t;
        [[nodiscard]] auto HasCallbackFunction(int functionId) const -> bool;
        [[nodiscard]] auto HasCallbackByDescription(const std::string& description) const -> bool;
        [[nodiscard]] auto HasTagFunction(int functionId) const -> bool;
        [[nodiscard]] auto HasRadarScreenTagFunction(int functionId) const -> bool;
        auto ReserveNextDynamicFunctionId() -> int;
        void RegisterFunctionCall(const UKControllerPlugin::Euroscope::CallbackFunction& function);
        void RegisterFunctionCall(const UKControllerPlugin::Tag::TagFunction& function);
        void RegisterFunctionCall(const UKControllerPlugin::Tag::RadarScreenTagFunction& function);
        void RegisterTagFunctionsWithEuroscope(
            UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin) const;

        private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };
} // namespace UKControllerPlugin::Plugin
