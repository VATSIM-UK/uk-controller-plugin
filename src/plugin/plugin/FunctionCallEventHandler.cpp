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

        if (functionId < this->firstFixedId) {
            auto function = this->callbackFunctions.find(functionId);
            if (function == this->callbackFunctions.cend()) {
                LogWarning("Invalid dynamic callback function " + std::to_string(functionId));
                return;
            }

            function->function(functionId, subject, area);
        } else {
            auto function = this->tagFunctions.find(functionId);
            if (function == this->tagFunctions.cend()) {
                LogWarning("Invalid fixed callback function " + std::to_string(functionId));
                return;
            }

            function->function(flightplan, radarTarget, subject, mousePos);
        }
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
        for (auto it = this->tagFunctions.cbegin(); it != this->tagFunctions.cend(); ++it) {
            plugin.RegisterTagFunction(it->functionId, it->description);
        }
    }

    /*
        Registers a CallbackFunction with the handlers.
    */
    void FunctionCallEventHandler::RegisterFunctionCall(const CallbackFunction& function)
    {
        if (!this->callbackFunctions.insert(function).second) {
            throw std::invalid_argument("Function already registered for this id!");
        }
    }

    /*
        Registers a TagFunction with the handlers.
    */
    void FunctionCallEventHandler::RegisterFunctionCall(const UKControllerPlugin::Tag::TagFunction& function)
    {
        if (!this->tagFunctions.insert(function).second) {
            throw std::invalid_argument("Function already registered for this id!");
        }
    }
    
    auto FunctionCallEventHandler::HasCallbackByDescription(const std::string& description) const -> bool
    {
        return std::find_if(
                   this->callbackFunctions.begin(),
                   this->callbackFunctions.end(),
                   [description](const Euroscope::CallbackFunction& callback) -> bool {
                       return callback.description == description;
                   }) != this->callbackFunctions.cend();
    }
} // namespace UKControllerPlugin::Plugin
