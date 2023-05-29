#include "plugin/FunctionCallEventHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Tag::TagFunction;

namespace UKControllerPlugin::Plugin {

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
        auto callbackFunction = this->callbackFunctions.find(functionId);
        if (callbackFunction != this->callbackFunctions.cend()) {
            callbackFunction->second.function(functionId, subject, area);
            return;
        }

        auto tagFunction = this->tagFunctions.find(functionId);
        if (tagFunction != this->tagFunctions.cend()) {
            tagFunction->second.function(flightplan, radarTarget, subject, mousePos);
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
        auto tagFunction = this->radarScreenTagFunctions.find(functionId);
        if (tagFunction == this->radarScreenTagFunctions.cend()) {
            return;
        }

        tagFunction->second.function(radarScreen, flightplan, radarTarget, subject, mousePos, area);
    }

    /*
        Returns the number of registered functions.
    */
    auto FunctionCallEventHandler::CountCallbacks() const -> size_t
    {
        return this->callbackFunctions.size();
    }

    /*
        Returns the number of tag functions.
    */
    auto FunctionCallEventHandler::CountTagFunctions() const -> size_t
    {
        return this->tagFunctions.size();
    }

    /*
        Returns true if a callback function exists
    */
    auto FunctionCallEventHandler::HasCallbackFunction(int id) const -> bool
    {
        return this->callbackFunctions.find(id) != this->callbackFunctions.cend();
    }

    /*
        Returns true if a tag function exists
    */
    auto FunctionCallEventHandler::HasTagFunction(int id) const -> bool
    {
        return this->tagFunctions.find(id) != this->tagFunctions.cend();
    }

    /*
        Returns the next function ID.
    */
    auto FunctionCallEventHandler::ReserveNextDynamicFunctionId() -> int
    {
        return this->nextDynamicFunctionId++;
    }

    /*
        Causes the collection to register all its TAG functions with the EuroScope plugin.
    */
    void FunctionCallEventHandler::RegisterTagFunctionsWithEuroscope(EuroscopePluginLoopbackInterface& plugin) const
    {
        // Register the regular TAG functions
        for (const auto& tagFunction : tagFunctions) {
            plugin.RegisterTagFunction(tagFunction.first, tagFunction.second.description);
        }

        // Register the radar screen TAG functions
        for (const auto& tagFunction : radarScreenTagFunctions) {
            plugin.RegisterTagFunction(tagFunction.first, tagFunction.second.description);
        }
    }

    /*
        Registers a CallbackFunction with the handlers.
    */
    void FunctionCallEventHandler::RegisterFunctionCall(const CallbackFunction& function)
    {
        if (this->callbackFunctions.contains(function.functionId)) {
            throw std::invalid_argument("Function already registered for this id!");
        }

        this->callbackFunctions[function.functionId] = function;
    }

    /*
        Registers a TagFunction with the handlers.
    */
    void FunctionCallEventHandler::RegisterFunctionCall(const UKControllerPlugin::Tag::TagFunction& function)
    {
        if (this->tagFunctions.contains(function.functionId)) {
            throw std::invalid_argument("Function already registered for this id!");
        }

        this->tagFunctions[function.functionId] = function;
    }

    auto FunctionCallEventHandler::HasCallbackByDescription(const std::string& description) const -> bool
    {
        for (const auto& callback : this->callbackFunctions) {
            if (callback.second.description == description) {
                return true;
            }
        }

        return false;
    }

    void FunctionCallEventHandler::RegisterFunctionCall(const Tag::RadarScreenTagFunction& function)
    {
        if (HasRadarScreenTagFunction(function.functionId)) {
            throw std::invalid_argument("Function already registered for this id!");
        }

        this->radarScreenTagFunctions[function.functionId] = function;
    }

    auto FunctionCallEventHandler::HasRadarScreenTagFunction(int id) const -> bool
    {
        return this->radarScreenTagFunctions.contains(id);
    }

    auto FunctionCallEventHandler::CountRadarScreenTagFunctions() const -> size_t
    {
        return this->radarScreenTagFunctions.size();
    }
} // namespace UKControllerPlugin::Plugin
