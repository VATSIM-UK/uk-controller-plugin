#include "CallsignSelectionList.h"
#include "CallsignSelectionListFactory.h"
#include "CallsignSelectionProviderInterface.h"
#include "euroscope/CallbackFunction.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "plugin/FunctionCallEventHandler.h"

namespace UKControllerPlugin::Aircraft {

    auto Create(
        std::shared_ptr<CallsignSelectionProviderInterface> provider,
        Plugin::FunctionCallEventHandler& functionHandler,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        const std::string& description) -> std::shared_ptr<CallsignSelectionList>
    {
        int callbackId = functionHandler.ReserveNextDynamicFunctionId();
        auto selectionList = std::make_shared<CallsignSelectionList>(std::move(provider), plugin, callbackId);

        Euroscope::CallbackFunction callback(
            callbackId, description, [selectionList](int functionId, std::string subject, RECT screenObjectArea) {
                selectionList->CallsignSelected(subject);
            });
        functionHandler.RegisterFunctionCall(callback);

        return selectionList;
    }
} // namespace UKControllerPlugin::Aircraft
