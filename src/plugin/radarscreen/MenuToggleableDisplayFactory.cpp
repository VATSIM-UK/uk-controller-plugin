#include "MenuToggleableDisplayFactory.h"
#include "ToggleDisplayFromMenu.h"
#include "euroscope/CallbackFunction.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

namespace UKControllerPlugin::RadarScreen {

    MenuToggleableDisplayFactory::MenuToggleableDisplayFactory(
        Plugin::FunctionCallEventHandler& callbacks, ConfigurableDisplayCollection& configurables)
        : callbacks(callbacks), configurables(configurables)
    {
    }

    void MenuToggleableDisplayFactory::RegisterDisplay(
        std::shared_ptr<MenuToggleableDisplayInterface> display, std::string description) const
    {
        const int callbackId = callbacks.ReserveNextDynamicFunctionId();
        auto toggle = std::make_shared<ToggleDisplayFromMenu>(display, callbackId);

        callbacks.RegisterFunctionCall(Euroscope::CallbackFunction(
            callbackId, description, [toggle](int functionId, const std::string& subject, RECT screenObjectArea) {
                toggle->Configure(functionId, subject, screenObjectArea);
            }));
        configurables.RegisterDisplay(toggle);
    }
} // namespace UKControllerPlugin::RadarScreen
