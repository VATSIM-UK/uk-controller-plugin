#pragma once
#include "euroscope/CallbackFunction.h"
#include "euroscope/CompareFunctions.h"
#include "tag/TagFunction.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
namespace Plugin {

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
            std::string subject,
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
        ) const;
        size_t CountCallbacks(void) const;
        size_t CountTagFunctions(void) const;
        bool HasCallbackFunction(int id) const;
        bool HasTagFunction(int id) const;
        int ReserveNextDynamicFunctionId(void);
        void RegisterFunctionCall(const UKControllerPlugin::Euroscope::CallbackFunction function);
        void RegisterFunctionCall(const UKControllerPlugin::Tag::TagFunction function);
        void RegisterTagFunctionsWithEuroscope(
            UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin
        ) const;

    private:

        /*
            The next available dynamic function ID. These ids can be used by any function that doesn't
            need to have the same id every time the plugin runs. For example, these would be perfect for
            a callback on a popup menu item. These wouldn't be useful for, say, a TAG function
            - which is probably saved in the settings somewhere.
        */
        int nextDynamicFunctionId = 5000;

        /*
            The first function id that is fixed - will always have one purpose each time the plugin loads.
        */
        const int firstFixedId = 9000;

        // The registered functions which have dynamic ids - may be different on each load.
        std::set<UKControllerPlugin::Euroscope::CallbackFunction, UKControllerPlugin::Euroscope::CompareFunctions>
            callbackFunctions;

        // The registered functions which have fixed ids - always the same on every load, as defined in the wiki.
        std::set<UKControllerPlugin::Tag::TagFunction, UKControllerPlugin::Euroscope::CompareFunctions> tagFunctions;
};
}  // namespace Plugin
}  // namespace UKControllerPlugin
