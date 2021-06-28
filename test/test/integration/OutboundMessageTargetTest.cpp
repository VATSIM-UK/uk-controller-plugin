#include "pch/pch.h"
#include "integration/OutboundMessageTarget.h"

using testing::Test;
using UKControllerPlugin::Integration::OutboundMessageTarget;

namespace UKControllerPluginTest::Integration {

    class OutboundMessageTargetTest : public Test
    {
        public:
            OutboundMessageTargetTest()
                : target(windowName, messageDataTypeIdentifier, interestedMessages)
            { }

            std::wstring windowName = L"TestWindow";
            ULONG_PTR messageDataTypeIdentifier = 1234;
            std::set<std::string> interestedMessages = {"test1", "test2", "test3"};
            OutboundMessageTarget target;
    };

    TEST_F(OutboundMessageTargetTest, ItHasAWindowName)
    {
        EXPECT_EQ(windowName, target.WindowName());
    }

    TEST_F(OutboundMessageTargetTest, IdentifierMatchesWindowName)
    {
        EXPECT_EQ(windowName, target.Identifier());
    }

    TEST_F(OutboundMessageTargetTest, ItHasADataTypeIdentifier)
    {
        EXPECT_EQ(messageDataTypeIdentifier, target.MessageDataTypeIdentifier());
    }

    TEST_F(OutboundMessageTargetTest, ItHasInterestedMessages)
    {
        EXPECT_EQ(interestedMessages, target.InterestedMessages());
    }

    TEST_F(OutboundMessageTargetTest, ItIsInterestedInAMessage)
    {
        EXPECT_TRUE(target.InterestedInMessage("test1"));
    }

    TEST_F(OutboundMessageTargetTest, ItIsNotInterestedInAMessage)
    {
        EXPECT_FALSE(target.InterestedInMessage("test5"));
    }

    TEST_F(OutboundMessageTargetTest, ItIsEqualForSameIdentifier)
    {
        OutboundMessageTarget target2(windowName, 1111, {"abc"});
        EXPECT_EQ(target, target2);
    }

    TEST_F(OutboundMessageTargetTest, ItIsNotEqualForSameIdentifier)
    {
        OutboundMessageTarget target2(L"notthiswindow", 1111, {"abc"});
        EXPECT_NE(target, target2);
    }
} // namespace UKControllerPluginTest::Integration
