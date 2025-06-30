#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class UserSetting;
    } // namespace Euroscope
    namespace Windows {
        struct GdiplusBrushes;
    } // namespace Windows
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Graphics {

    class ThemingModule
    {
    public:
        static void BootstrapPlugin(Bootstrap::PersistenceContainer& pc, Euroscope::UserSetting& userSetting);
        static void ApplyTheme(const std::string& paletteName, Windows::GdiplusBrushes& brushes);
    };

}