#pragma once

namespace UKControllerPlugin::RadarScreen {
    /**
     * An interface for classes that want to have a visibility toggle
     * in the configuration menu.
     */
    class MenuToggleableDisplayInterface
    {
        public:
        virtual ~MenuToggleableDisplayInterface() = default;
        [[nodiscard]] virtual auto MenuItem() const -> std::string = 0;
        [[nodiscard]] virtual auto IsVisible() const -> bool = 0;
        virtual void Toggle() = 0;
    };
} // namespace UKControllerPlugin::RadarScreen
