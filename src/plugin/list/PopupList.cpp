#include "ListItem.h"
#include "ListItemProviderInterface.h"
#include "PopupList.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPlugin::List {

    PopupList::PopupList(
        std::shared_ptr<ListItemProviderInterface> itemProvider,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        int callbackId)
        : itemProvider(std::move(itemProvider)), plugin(plugin), callbackId(callbackId)
    {
    }

    void PopupList::Trigger(const POINT& location)
    {
        const auto items = itemProvider->ListItems();
        if (items.empty()) {
            return;
        }

        this->plugin.TriggerPopupList(
            {location.x, location.y, location.x + 200, location.y + 400},
            itemProvider->ListName(),
            itemProvider->ListColumns());

        for (const auto& item : items) {
            PopupMenuItem popupItem;
            popupItem.firstValue = item->firstColumn;
            popupItem.secondValue = item->secondColumn;
            popupItem.checked = static_cast<unsigned int>(item->checked);
            popupItem.disabled = item->disabled;
            popupItem.callbackFunctionId = callbackId;

            this->plugin.AddItemToPopupList(popupItem);
        }
    }
} // namespace UKControllerPlugin::List
