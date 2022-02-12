#include "aircraft/CallsignSelectionList.h"
#include "list/ListItem.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::Aircraft::CallsignSelectionList;
using UKControllerPlugin::List::ListItem;
using UKControllerPlugin::List::ListItemCheckedStatus;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPluginTest::Aircraft {
    class CallsignSelectionListTest : public testing::Test
    {
        public:
        CallsignSelectionListTest()
            : callsignProvider(std::make_shared<testing::NiceMock<Aircraft::MockCallsignSelectionProvider>>()),
              list(callsignProvider)
        {
        }
        std::shared_ptr<testing::NiceMock<Aircraft::MockCallsignSelectionProvider>> callsignProvider;
        CallsignSelectionList list;
    };

    TEST_F(CallsignSelectionListTest, ItHasASingleColumn)
    {
        EXPECT_EQ(1, list.ListColumns());
    }

    TEST_F(CallsignSelectionListTest, ItHasAName)
    {
        EXPECT_EQ("Select Aircraft", list.ListName());
    }

    TEST_F(CallsignSelectionListTest, ItReturnsListItems)
    {
        EXPECT_CALL(*callsignProvider, GetCallsigns)
            .Times(1)
            .WillOnce(testing::Return(std::set<std::string>{"ABCD", "EFGH", "IJKL"}));

        const auto items = list.ListItems();
        EXPECT_EQ(3, items.size());

        auto itemIterator = items.cbegin();
        const auto item1 = *itemIterator++;
        EXPECT_EQ("ABCD", item1->firstColumn);
        EXPECT_EQ("", item1->secondColumn);
        EXPECT_FALSE(item1->fixedPosition);
        EXPECT_FALSE(item1->disabled);
        EXPECT_EQ(ListItemCheckedStatus::NoCheckbox, item1->checked);

        const auto item2 = *itemIterator++;
        EXPECT_EQ("EFGH", item2->firstColumn);
        EXPECT_EQ("", item2->secondColumn);
        EXPECT_FALSE(item2->fixedPosition);
        EXPECT_FALSE(item2->disabled);
        EXPECT_EQ(ListItemCheckedStatus::NoCheckbox, item2->checked);

        const auto item3 = *itemIterator++;
        EXPECT_EQ("IJKL", item3->firstColumn);
        EXPECT_EQ("", item3->secondColumn);
        EXPECT_FALSE(item3->fixedPosition);
        EXPECT_FALSE(item3->disabled);
        EXPECT_EQ(ListItemCheckedStatus::NoCheckbox, item3->checked);
    }

    TEST_F(CallsignSelectionListTest, ItTriggersCallsignSelected)
    {
        EXPECT_CALL(*callsignProvider, CallsignSelected("BAW123")).Times(1);

        list.ItemSelected("BAW123");
    }
} // namespace UKControllerPluginTest::Aircraft
