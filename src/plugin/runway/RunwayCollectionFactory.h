#pragma once

namespace UKControllerPlugin {
    namespace Airfield {
        class AirfieldCollection;
    } // namespace Airfield
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Dependency {
        class DependencyLoaderInterface;
    } // namespace Dependency
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Runway {
    class RunwayCollection;

    auto BuildRunwayCollection(const nlohmann::json& dependency, const Airfield::AirfieldCollection& airfields)
        -> std::shared_ptr<RunwayCollection>;
    auto RunwayValid(const nlohmann::json& runway, const Airfield::AirfieldCollection& airfields) -> bool;
} // namespace UKControllerPlugin::Runway
