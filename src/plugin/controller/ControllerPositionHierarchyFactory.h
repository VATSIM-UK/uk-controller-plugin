#pragma once

namespace UKControllerPlugin::Controller {
    class ControllerPositionCollection;
    class ControllerPositionHierarchy;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Controller {

    /*
        Responsible for creating Hierarchies of controllers.
    */
    class ControllerPositionHierarchyFactory
    {
        public:
        ControllerPositionHierarchyFactory(const ControllerPositionCollection& controllers);
        [[nodiscard]] std::unique_ptr<ControllerPositionHierarchy>
        CreateFromJsonByCallsign(const nlohmann::json& json) const;
        [[nodiscard]] std::unique_ptr<ControllerPositionHierarchy> CreateFromJsonById(const nlohmann::json& json) const;

        private:
        // All the controller positions known to the plugin
        const ControllerPositionCollection& controllers;
    };
} // namespace UKControllerPlugin::Controller
