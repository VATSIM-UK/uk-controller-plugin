#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
} // namespace UKControllerPlugin
// END

/*

*/
namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            A class for bootstrapping externals, such as the windows API.
        */
        class ExternalsBootstrap
        {
            public:
            static void Bootstrap(UKControllerPlugin::Bootstrap::PersistenceContainer& persistence, HINSTANCE instance);

            static std::wstring GetPluginFileRoot(void);
            static void SetupUkcpFolderRoot(UKControllerPlugin::Windows::WinApiInterface& winApi);

            private:
            static std::wstring GetLocalAppDataPath(void);
            static std::wstring GetKnownFolderPath(GUID folderId);
        };
    } // namespace Bootstrap
} // namespace UKControllerPlugin
