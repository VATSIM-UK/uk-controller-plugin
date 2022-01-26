#pragma once

namespace UKControllerPlugin::Plugin {
    class FunctionCallEventHandler;
} // namespace UKControllerPlugin::Plugin

namespace UKControllerPlugin::RadarScreen {
    class ConfigurableDisplayCollection;
    class MenuToggleableDisplayInterface;

    class MenuToggleableDisplayFactory
    {
        public:
        MenuToggleableDisplayFactory(
            Plugin::FunctionCallEventHandler& callbacks, ConfigurableDisplayCollection& configurables);
        void RegisterDisplay(std::shared_ptr<MenuToggleableDisplayInterface> display, std::string description) const;

        private:
        // For registering the callback
        Plugin::FunctionCallEventHandler& callbacks;

        // For registering the configuration item
        ConfigurableDisplayCollection& configurables;
    };
} // namespace UKControllerPlugin::RadarScreen
