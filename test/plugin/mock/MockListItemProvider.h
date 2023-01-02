#pragma once
#include "list/ListItemProviderInterface.h"

using UKControllerPlugin::List::ListItem;
using UKControllerPlugin::List::ListItemProviderInterface;

namespace UKControllerPluginTest::List {
    class MockListItemProvider : public ListItemProviderInterface
    {
        public:
        MockListItemProvider();
        virtual ~MockListItemProvider();
        MOCK_METHOD(int, ListColumns, (), (override));
        MOCK_METHOD(std::string, ListName, (), (override));
        MOCK_METHOD(std::list<std::shared_ptr<ListItem>>, ListItems, (), (override));
        MOCK_METHOD(void, ItemSelected, (const std::string&), (override));
    };
} // namespace UKControllerPluginTest::List
