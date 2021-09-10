#include "ConfigurableDisplayInterface.h"

namespace UKControllerPlugin::RadarScreen {
    ConfigurableDisplayInterface::ConfigurableDisplayInterface() = default;
    ConfigurableDisplayInterface::~ConfigurableDisplayInterface() = default;
    ConfigurableDisplayInterface::ConfigurableDisplayInterface(const ConfigurableDisplayInterface&) = default;
    [[nodiscard]] auto ConfigurableDisplayInterface::operator=(const ConfigurableDisplayInterface&)
        -> ConfigurableDisplayInterface& = default;
} // namespace UKControllerPlugin::RadarScreen
