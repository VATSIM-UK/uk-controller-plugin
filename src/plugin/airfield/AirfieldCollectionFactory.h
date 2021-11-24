#pragma once

namespace UKControllerPlugin {
    namespace Controller {
        class ControllerPositionHierarchyFactory;
    } // namespace Controller
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Airfield {

    class AirfieldCollection;

    [[nodiscard]] auto
    CreateAirfieldCollection(const nlohmann::json& airfields, const Controller::ControllerPositionHierarchyFactory&
                                                                 hierarchyFactory)
        -> std::unique_ptr<const UKControllerPlugin::Airfield::AirfieldCollection>;

    [[nodiscard]] auto
    AirfieldValid(const nlohmann::json& airfield, const Controller::ControllerPositionHierarchyFactory& hierarchyFactory) ->
        bool;
    [[nodiscard]] auto
    AirfieldValid(const nlohmann::json& airfield, const Controller::ControllerPositionHierarchyFactory& hierarchyFactory) ->
        bool;
} // namespace UKControllerPlugin::Airfield
