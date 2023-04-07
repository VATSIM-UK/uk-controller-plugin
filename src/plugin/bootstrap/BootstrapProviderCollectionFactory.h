#pragma once

namespace UKControllerPlugin::Bootstrap {
    class BootstrapProviderCollection;

    [[nodiscard]] auto Make() -> std::unique_ptr<BootstrapProviderCollection>;
} // namespace UKControllerPlugin::Bootstrap
