#pragma once
#include "api/ApiKeyReceivedEvent.h"
#include "eventhandler/EventHandler.h"

namespace UKControllerPlugin::Windows {
    class WinApiInterface;
} // namespace UKControllerPlugin::Windows

namespace UKControllerPluginUtils::Api {
    class ApiConfigurationListener;
    class ApiKeyRedirectUrlBuilder;
} // namespace UKControllerPluginUtils::Api

namespace UKControllerPlugin::Api {

    /*
        A dialog for replacing the API key.
    */
    class ReplaceApiKeyDialog : public EventHandler::EventHandler<ApiKeyReceivedEvent>
    {
        public:
        ReplaceApiKeyDialog(
            std::unique_ptr<UKControllerPluginUtils::Api::ApiKeyRedirectUrlBuilder> urlBuilder,
            UKControllerPlugin::Windows::WinApiInterface& windows);
        ~ReplaceApiKeyDialog() = default;
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

        private:
        LRESULT _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT InitDialog();
        void DestroyDialog();
        void OpenBrowserWindow();

        // Api settings
        std::unique_ptr<UKControllerPluginUtils::Api::ApiKeyRedirectUrlBuilder> urlBuilder;

        // Windows
        UKControllerPlugin::Windows::WinApiInterface& windows;

        // Listnen for api config changes
        std::shared_ptr<UKControllerPluginUtils::Api::ApiConfigurationListener> apiConfigListener;
    };
} // namespace UKControllerPlugin::Api
