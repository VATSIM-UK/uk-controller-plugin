#include "plugin/FunctionCallEventHandler.h"
#include "euroscope/CallbackFunction.h"
#include "euroscope/RadarScreenCallbackFunction.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/RadarScreenCallbackFunction.h"
#include "tag/TagFunction.h"
#include "tag/RadarScreenTagFunction.h"

using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Tag::TagFunction;

namespace UKControllerPlugin::Plugin {

    struct FunctionCallEventHandler::Impl
    {
        static const int INITIAL_DYNAMIC_FUNCTION_ID = 10000;

        /*
            The next available dynamic function ID. These ids can be used by any function that doesn't
            need to have the same id every time the plugin runs. For example, these would be perfect for
            a callback on a popup menu item. These wouldn't be useful for, say, a TAG function
            - which is probably saved in the settings somewhere.
        */
        int nextDynamicFunctionId = INITIAL_DYNAMIC_FUNCTION_ID;

        // The registered functions which have dynamic ids - may be different on each load.
        std::unordered_map<int, UKControllerPlugin::Euroscope::CallbackFunction> callbackFunctions;
        std::unordered_map<int, UKControllerPlugin::Euroscope::RadarScreenCallbackFunction> radarScreenCallbacks;

        // The registered functions which have fixed ids - always the same on every load, as defined in the wiki.
        std::unordered_map<int, UKControllerPlugin::Tag::TagFunction> tagFunctions;

        // The registered functions which have fixed ids - always the same on every load, as defined in the wiki.
        // This is for functions called at the RadarScreen level.
        std::unordered_map<int, UKControllerPlugin::Tag::RadarScreenTagFunction> radarScreenTagFunctions;
    };

    FunctionCallEventHandler::FunctionCallEventHandler() : impl(std::make_unique<Impl>())
    {
    }

    /*
        Call the given function.
    */
    void FunctionCallEventHandler::CallFunction(
        int functionId,
        const std::string& subject,
        EuroScopeCFlightPlanInterface& flightplan,
        EuroScopeCRadarTargetInterface& radarTarget,
        const POINT& mousePos,
        const RECT& area) const
    {
        auto callbackFunction = this->impl->callbackFunctions.find(functionId);
        if (callbackFunction != this->impl->callbackFunctions.cend()) {
            try {
                callbackFunction->second.function(functionId, subject, area);
            } catch (const std::exception& e) {
                LogFatalExceptionAndRethrow(
                    "FunctionCallEventHandler::CallFunction::Plugin::Callback",
                    callbackFunction->second.description,
                    e);
            }
            return;
        }

        auto tagFunction = this->impl->tagFunctions.find(functionId);
        if (tagFunction != this->impl->tagFunctions.cend()) {
            try {
                tagFunction->second.function(flightplan, radarTarget, subject, mousePos);
            } catch (const std::exception& e) {
                LogFatalExceptionAndRethrow(
                    "FunctionCallEventHandler::CallFunction::Plugin::Tag", tagFunction->second.description, e);
            }
            return;
        }
    }

    void FunctionCallEventHandler::CallFunction(
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
        int functionId,
        const std::string& subject,
        EuroScopeCFlightPlanInterface& flightplan,
        EuroScopeCRadarTargetInterface& radarTarget,
        const POINT& mousePos,
        const RECT& area) const
    {
        auto tagFunction = this->impl->radarScreenTagFunctions.find(functionId);
        if (tagFunction != this->impl->radarScreenTagFunctions.cend()) {
            try {
                tagFunction->second.function(radarScreen, flightplan, radarTarget, subject, mousePos, area);
            } catch (const std::exception& e) {
                LogFatalExceptionAndRethrow(
                    "FunctionCallEventHandler::CallFunction::RadarScreen::Tag", tagFunction->second.description, e);
            }
            return;
        }

        auto callbackFunction = this->impl->radarScreenCallbacks.find(functionId);
        if (callbackFunction != this->impl->radarScreenCallbacks.cend()) {
            try {
                callbackFunction->second.function(functionId, radarScreen, subject, mousePos, area);
            } catch (const std::exception& e) {
                LogFatalExceptionAndRethrow(
                    "FunctionCallEventHandler::CallFunction::RadarScreen::Callback",
                    callbackFunction->second.description,
                    e);
            }
            return;
        }
    }

    FunctionCallEventHandler::~FunctionCallEventHandler() = default;

    /*
        Returns the number of registered functions.
    */
    auto FunctionCallEventHandler::CountCallbacks() const -> size_t
    {
        return this->impl->callbackFunctions.size();
    }

    /*
        Returns the number of tag functions.
    */
    auto FunctionCallEventHandler::CountTagFunctions() const -> size_t
    {
        return this->impl->tagFunctions.size();
    }

    /*
        Returns true if a callback function exists
    */
    auto FunctionCallEventHandler::HasCallbackFunction(int functionId) const -> bool
    {
        return this->impl->callbackFunctions.find(functionId) != this->impl->callbackFunctions.cend();
    }

    /*
        Returns true if a tag function exists
    */
    auto FunctionCallEventHandler::HasTagFunction(int functionId) const -> bool
    {
        return this->impl->tagFunctions.find(functionId) != this->impl->tagFunctions.cend();
    }

    /*
        Returns the next function ID.
    */
    auto FunctionCallEventHandler::ReserveNextDynamicFunctionId() -> int
    {
        return this->impl->nextDynamicFunctionId++;
    }

    /*
        Causes the collection to register all its TAG functions with the EuroScope plugin.
    */
    void FunctionCallEventHandler::RegisterTagFunctionsWithEuroscope(EuroscopePluginLoopbackInterface& plugin) const
    {
        // Register the regular TAG functions
        for (const auto& tagFunction : this->impl->tagFunctions) {
            plugin.RegisterTagFunction(tagFunction.first, tagFunction.second.description);
        }

        // Register the radar screen TAG functions
        for (const auto& tagFunction : this->impl->radarScreenTagFunctions) {
            plugin.RegisterTagFunction(tagFunction.first, tagFunction.second.description);
        }
    }

    /*
        Registers a CallbackFunction with the handlers.
    */
    void FunctionCallEventHandler::RegisterFunctionCall(const CallbackFunction& function)
    {
        if (this->impl->callbackFunctions.contains(function.functionId)) {
            throw std::invalid_argument("CallbackFunction already registered for this id!");
        }

        this->impl->callbackFunctions[function.functionId] = function;
    }

    /*
    Registers a RadarScreenCallbackFunction with the handlers.
*/
    void FunctionCallEventHandler::RegisterFunctionCall(const Euroscope::RadarScreenCallbackFunction& function)
    {
        if (this->impl->radarScreenCallbacks.contains(function.functionId)) {
            throw std::invalid_argument("RadarScreenCallbackFunction already registered for this id!");
        }

        this->impl->radarScreenCallbacks[function.functionId] = function;
    }

    /*
        Registers a TagFunction with the handlers.
    */
    void FunctionCallEventHandler::RegisterFunctionCall(const UKControllerPlugin::Tag::TagFunction& function)
    {
        if (this->impl->tagFunctions.contains(function.functionId)) {
            throw std::invalid_argument("TagFunction already registered for this id!");
        }

        this->impl->tagFunctions[function.functionId] = function;
    }

    auto FunctionCallEventHandler::HasCallbackByDescription(const std::string& description) const -> bool
    {
        return std::any_of(
            this->impl->callbackFunctions.cbegin(),
            this->impl->callbackFunctions.cend(),
            [&description](const auto& callback) { return callback.second.description == description; });
    }

    void FunctionCallEventHandler::RegisterFunctionCall(const Tag::RadarScreenTagFunction& function)
    {
        if (HasRadarScreenTagFunction(function.functionId)) {
            throw std::invalid_argument("RadarScreenTagFunction already registered for this id!");
        }

        this->impl->radarScreenTagFunctions[function.functionId] = function;
    }

    auto FunctionCallEventHandler::HasRadarScreenTagFunction(int functionId) const -> bool
    {
        return this->impl->radarScreenTagFunctions.contains(functionId);
    }

    auto FunctionCallEventHandler::CountRadarScreenTagFunctions() const -> size_t
    {
        return this->impl->radarScreenTagFunctions.size();
    }

    auto FunctionCallEventHandler::HasRadarScreenCallbackFunction(int functionId) const -> bool
    {
        return this->impl->radarScreenCallbacks.contains(functionId);
    }

    auto FunctionCallEventHandler::HasRadarScreenCallbackByDescription(const std::string& description) const -> bool
    {
        return std::any_of(
            this->impl->radarScreenCallbacks.cbegin(),
            this->impl->radarScreenCallbacks.cend(),
            [&description](const auto& callback) { return callback.second.description == description; });
    }

    auto FunctionCallEventHandler::CountRadarScreenCallbacks() const -> size_t
    {
        return this->impl->radarScreenCallbacks.size();
    }
} // namespace UKControllerPlugin::Plugin
