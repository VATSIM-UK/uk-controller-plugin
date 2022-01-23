#pragma once

namespace UKControllerPlugin::List {
    struct ListItem;

    /**
     * An interface for a class that provides items for a popup list.
     */
    class ListItemProviderInterface
    {
        public:
        virtual ~ListItemProviderInterface() = default;
        [[nodiscard]] virtual auto ListColumns() -> int = 0;
        [[nodiscard]] virtual auto ListName() -> std::string = 0;
        [[nodiscard]] virtual auto ListItems() -> std::list<std::shared_ptr<ListItem>> = 0;
        virtual void ItemSelected(const std::string& item) = 0;
    };
} // namespace UKControllerPlugin::List
