#pragma once
#include "PopupListInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::List {
    class ListItemProviderInterface;

    /*
     * A class that wraps EuroScopes popup list.
     */
    class PopupList : public PopupListInterface
    {
        public:
        PopupList(
            std::shared_ptr<ListItemProviderInterface> itemProvider,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            int callbackId);
        void Trigger(const POINT& location) override;

        private:
        // Provider of list data
        std::shared_ptr<ListItemProviderInterface> itemProvider;

        // Plugin for triggering the list
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // The callback id for this list when an item is selected
        int callbackId;
    };
} // namespace UKControllerPlugin::List
