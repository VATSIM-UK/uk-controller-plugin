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
        [[nodiscard]] std::shared_ptr<ControllerPositionHierarchy>
        CreateSharedFromJsonById(const nlohmann::json& json) const;
        [[nodiscard]] auto GetPositionsCollection() const -> const ControllerPositionCollection&;

        private:
        [[nodiscard]] auto
        AddPositionsById(const nlohmann::json& positions, ControllerPositionHierarchy* const hierarchy) const -> bool;

        // All the controller positions known to the plugin
        const ControllerPositionCollection& controllers;
    };
} // namespace UKControllerPlugin::Controller
