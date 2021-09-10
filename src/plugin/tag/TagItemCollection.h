#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Tag {
        class TagData;
        class TagItemInterface;
    } // namespace Tag
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Tag {

    /*
        A collection of TAG items, which can be called to
        produce data for a TAG.
    */
    class TagItemCollection
    {
        public:
        [[nodiscard]] auto CountHandlers() const -> size_t;
        [[nodiscard]] auto HasHandlerForItemId(int id) const -> bool;
        void RegisterTagItem(int itemId, std::shared_ptr<UKControllerPlugin::Tag::TagItemInterface> tagItem);
        void TagItemUpdate(UKControllerPlugin::Tag::TagData& tagData) const;
        void RegisterAllItemsWithEuroscope(
            UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& pluginCore) const;

        private:
        
        // The tag item string to use if the tag item requested is invalid
        const std::string errorTagItemText = "ERROR";
        
        // All registered tag items
        std::map<int, std::shared_ptr<UKControllerPlugin::Tag::TagItemInterface>> tagItems;
    };
} // namespace UKControllerPlugin::Tag
