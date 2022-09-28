#include "ConfigurableDisplayCallbackFactory.h"
#include "ConfigurableDisplayInterface.h"

namespace UKControllerPlugin::RadarScreen {

    auto CreateConfigurableDisplayCallback(
        int id, const std::string& name, std::shared_ptr<ConfigurableDisplayInterface> display)
        -> Euroscope::CallbackFunction
    {
        return Euroscope::CallbackFunction(
            id, name, [display](int functionId, std::string subject, RECT screenObjectArea) {
                display->Configure(functionId, std::move(subject), screenObjectArea);
            });
        ;
    }
} // namespace UKControllerPlugin::RadarScreen
