#pragma once
#include "euroscope/CallbackFunction.h"
#include "tag/RadarScreenTagFunction.h"
#include "tag/TagFunction.h"

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
    class EuroScopeCRadarTargetInterface;
    class EuroscopePluginLoopbackInterface;
    class EuroscopeRadarLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Plugin {

    /*
        Class to handle Euroscope OnFunctionCall
        events. These events could be raised by CRadarScreen
        and CPlugin derivatives.
    */
    class FunctionCallEventHandler
    {
        public:
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
        [[nodiscard]] auto HasCallbackFunction(int id) const -> bool;
        [[nodiscard]] auto HasCallbackByDescription(const std::string& description) const -> bool;
        [[nodiscard]] auto HasTagFunction(int id) const -> bool;
        [[nodiscard]] auto HasRadarScreenTagFunction(int id) const -> bool;
        auto ReserveNextDynamicFunctionId() -> int;
        void RegisterFunctionCall(const UKControllerPlugin::Euroscope::CallbackFunction& function);
        void RegisterFunctionCall(const UKControllerPlugin::Tag::TagFunction& function);
        void RegisterFunctionCall(const UKControllerPlugin::Tag::RadarScreenTagFunction& function);
        void RegisterTagFunctionsWithEuroscope(
            UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin) const;

        private:
        /*
            The next available dynamic function ID. These ids can be used by any function that doesn't
            need to have the same id every time the plugin runs. For example, these would be perfect for
            a callback on a popup menu item. These wouldn't be useful for, say, a TAG function
            - which is probably saved in the settings somewhere.
        */
        int nextDynamicFunctionId = 10000;

        // The registered functions which have dynamic ids - may be different on each load.
        std::unordered_map<int, UKControllerPlugin::Euroscope::CallbackFunction> callbackFunctions;

        // The registered functions which have fixed ids - always the same on every load, as defined in the wiki.
        std::unordered_map<int, UKControllerPlugin::Tag::TagFunction> tagFunctions;

        // The registered functions which have fixed ids - always the same on every load, as defined in the wiki.
        // This is for functions called at the RadarScreen level.
        std::unordered_map<int, UKControllerPlugin::Tag::RadarScreenTagFunction> radarScreenTagFunctions;
    };
} // namespace UKControllerPlugin::Plugin
