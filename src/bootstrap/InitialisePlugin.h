#pragma once
#include "bootstrap/PersistenceContainer.h"
#include "duplicate/DuplicatePlugin.h"

// Forward Declarations
namespace EuroScopePlugIn {
    class CPlugIn;
}  // namespace EuroScopePlugIn
namespace UKControllerPlugin {
    class UKPlugin;
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {

    /*
        Class used to initialise all the elements of the plugin that are shared among
        the various modules, to fetch resources and check for updates when the plugin
        starts.
    */
    class InitialisePlugin
    {
        public:
            EuroScopePlugIn::CPlugIn * GetPlugin(void);
            void PostInit(HINSTANCE dllInstance);
            void EuroScopeCleanup(void);

        private:

            void CreateDummy(void);


            // In the event of an invalid version, this is the "dummy" fallback plugin.
            std::unique_ptr<EuroScopePlugIn::CPlugIn> fallbackPlugin;

            std::unique_ptr<UKControllerPlugin::Bootstrap::PersistenceContainer> container;

            // Whether or not the plugin has already been loaded - indicating this is a second ES window
            std::unique_ptr<Duplicate::DuplicatePlugin> duplicatePlugin;

            // Gdiplus initialisation token.
            ULONG_PTR gdiPlusToken;

            // Is winsocks initialised
            bool winsockInitialised;
    };  // namespace UKControllerPlugin
}  // namespace UKControllerPlugin
