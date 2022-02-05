#pragma once
#include "list/ListItemProviderInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Aircraft {
    class CallsignSelectionProviderInterface;

    /**
     * Implements a callsign selection list.
     */
    class CallsignSelectionList : public List::ListItemProviderInterface
    {
        public:
        CallsignSelectionList(std::shared_ptr<CallsignSelectionProviderInterface> callsignProvider);
        ~CallsignSelectionList() = default;
        auto ListColumns() -> int override;
        auto ListName() -> std::string override;
        auto ListItems() -> std::list<std::shared_ptr<List::ListItem>> override;
        void ItemSelected(const std::string& item) override;

        private:
        // Provides the callsigns
        std::shared_ptr<CallsignSelectionProviderInterface> callsignProvider;
    };
} // namespace UKControllerPlugin::Aircraft
