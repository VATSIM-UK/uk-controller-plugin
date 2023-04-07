#pragma once
#include "euroscope/CallbackFunction.h"

namespace UKControllerPlugin::RadarScreen {
    class ConfigurableDisplayInterface;

    [[nodiscard]] auto CreateConfigurableDisplayCallback(
        int id, const std::string& name, std::shared_ptr<ConfigurableDisplayInterface> display)
        -> Euroscope::CallbackFunction;
} // namespace UKControllerPlugin::RadarScreen
