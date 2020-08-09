#include "pch/stdafx.h"
#include "tag/TagItemCollection.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "tag/TagItemInterface.h"

using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;

namespace UKControllerPlugin {
    namespace Tag {

        /*
            Returns the number of registered handlers.
        */
        int TagItemCollection::CountHandlers(void) const
        {
            return this->tagItems.size();
        }

        /*
            Returns true if a handler is registered for a particular tagItem id.
        */
        bool TagItemCollection::HasHandlerForItemId(int id) const
        {
            return this->tagItems.count(id) > 0;
        }
        /*
            Registers a new TagItem with the collection. Registers it with Euroscope and increments
            the next tag item ID.
        */
        void TagItemCollection::RegisterTagItem(
            int itemId,
            std::shared_ptr<UKControllerPlugin::Tag::TagItemInterface> tagItem
        ) {
            if (this->tagItems.count(itemId) != 0) {
                throw std::invalid_argument("Tag item already exists!");
            }

            this->tagItems[itemId] = tagItem;
        }

        /*
            Called when we want to get data for a given tag item. For example, if we're looking
            for an intention code, the intention code tag item will be called to return data.
        */
        void TagItemCollection::TagItemUpdate(TagData & tagData) const
        {

            // Make sure the item exists
            if (this->tagItems.count(tagData.itemCode) == 0) {
                LogWarning("Invalid TAG item requested, id: " + std::to_string(tagData.itemCode));
                tagData.SetItemString(this->errorTagItemText);
                return;
            }

            // Set the data for the tag item
            this->tagItems.at(tagData.itemCode)->SetTagItemData(tagData);
        }

        /*
            Register every tag item in the collection with a EuroScope plugin instance.
        */
        void TagItemCollection::RegisterAllItemsWithEuroscope(EuroscopePluginLoopbackInterface & pluginCore) const
        {
            for (
                std::map<int, std::shared_ptr<UKControllerPlugin::Tag::TagItemInterface>>::const_iterator it =
                    this->tagItems.cbegin();
                it != this->tagItems.cend();
                ++it
            ) {
                pluginCore.RegisterTagItem(it->first, it->second->GetTagItemDescription(it->first));
            }

            LogInfo("Registered " + std::to_string(this->tagItems.size()) + " TAG items");
        }
    }  // namespace Tag
}  // namespace UKControllerPlugin
