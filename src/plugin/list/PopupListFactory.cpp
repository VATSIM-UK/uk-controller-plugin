#include "ListItemProviderInterface.h"
#include "PopupList.h"
#include "PopupListFactory.h"
#include "plugin/FunctionCallEventHandler.h"

namespace UKControllerPlugin::List {
    PopupListFactory::PopupListFactory(
        Plugin::FunctionCallEventHandler& functionHandler, Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : functionHandler(functionHandler), plugin(plugin)
    {
    }

    auto
    PopupListFactory::Create(std::shared_ptr<ListItemProviderInterface> provider, const std::string& description) const
        -> std::shared_ptr<PopupList>
    {
        const auto callbackId = functionHandler.ReserveNextDynamicFunctionId();
        auto selectionList = std::make_shared<PopupList>(std::move(provider), plugin, callbackId);

        Euroscope::CallbackFunction callback(
            callbackId, description, [provider](int functionId, const std::string& subject, RECT screenObjectArea) {
                provider->ItemSelected(subject);
            });
        functionHandler.RegisterFunctionCall(callback);

        return selectionList;
    }
} // namespace UKControllerPlugin::List
