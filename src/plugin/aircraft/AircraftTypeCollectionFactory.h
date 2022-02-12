#pragma once

namespace UKControllerPlugin::Aircraft {
    class AircraftTypeCollection;

    [[nodiscard]] auto CollectionFromDependency(const nlohmann::json& json) -> std::unique_ptr<AircraftTypeCollection>;
    [[nodiscard]] auto DependencyValid(const nlohmann::json& json) -> bool;
} // namespace UKControllerPlugin::Aircraft
