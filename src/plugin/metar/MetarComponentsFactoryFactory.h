#pragma once

namespace UKControllerPlugin::Metar {
    class MetarComponentsFactory;

    [[nodiscard]] auto BuildComponentsFactory() -> std::shared_ptr<MetarComponentsFactory>;
} // namespace UKControllerPlugin::Metar
