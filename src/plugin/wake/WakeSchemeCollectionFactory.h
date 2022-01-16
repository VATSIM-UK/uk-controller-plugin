#pragma once

namespace UKControllerPlugin::Wake {
    class WakeSchemeCollection;

    [[nodiscard]] auto CollectionFromDependency(const nlohmann::json& json) -> std::unique_ptr<WakeSchemeCollection>;
    [[nodiscard]] auto DependencyValid(const nlohmann::json& json) -> bool;
} // namespace UKControllerPlugin::Wake
