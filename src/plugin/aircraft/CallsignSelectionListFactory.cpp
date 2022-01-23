#include "CallsignSelectionList.h"
#include "CallsignSelectionListFactory.h"
#include "CallsignSelectionProviderInterface.h"
#include "euroscope/CallbackFunction.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "plugin/FunctionCallEventHandler.h"

namespace UKControllerPlugin::Aircraft {

    CallsignSelectionListFactory::CallsignSelectionListFactory(
        Plugin::FunctionCallEventHandler& functionHandler, Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : functionHandler(functionHandler), plugin(plugin)
    {
    }

    auto CallsignSelectionListFactory::Create(
        std::shared_ptr<CallsignSelectionProviderInterface> provider, const std::string& description) const
        -> std::shared_ptr<CallsignSelectionList>
    {
        int callbackId = functionHandler.ReserveNextDynamicFunctionId();
        auto selectionList = std::make_shared<CallsignSelectionList>(std::move(provider), plugin, callbackId);

        Euroscope::CallbackFunction callback(
            callbackId,
            description,
            [selectionList](int functionId, const std::string& subject, RECT screenObjectArea) {
                selectionList->CallsignSelected(subject);
            });
        functionHandler.RegisterFunctionCall(callback);

        return selectionList;
    }
} // namespace UKControllerPlugin::Aircraft
