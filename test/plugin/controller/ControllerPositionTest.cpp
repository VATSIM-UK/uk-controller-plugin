#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPluginTest::Controller {

    class ControllerPositionTest : public testing::Test
    {
        public:
        ControllerPositionTest()
            : controller(1, "EGFF_APP", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, false, false, true)
        {
        }

        ControllerPosition controller;
    };

    TEST_F(ControllerPositionTest, GetIdReturnsId)
    {
        EXPECT_EQ(1, controller.GetId());
    }

    TEST_F(ControllerPositionTest, GetCallsignReturnsCallsign)
    {
        EXPECT_EQ(0, controller.GetCallsign().compare("EGFF_APP"));
    }

    TEST_F(ControllerPositionTest, GetFrequencyReturnsFrequency)
    {
        EXPECT_EQ(125.850, controller.GetFrequency());
    }

    TEST_F(ControllerPositionTest, GetTypeReturnsType)
    {
        EXPECT_EQ(0, controller.GetType().compare("APP"));
    }

    TEST_F(ControllerPositionTest, GetTopdownReturnsTopdown)
    {
        std::vector<std::string> expected = {"EGGD", "EGFF"};
        EXPECT_EQ(expected, controller.GetTopdown());
    }

    TEST_F(ControllerPositionTest, RequestsDepartureReleasesReturnsWhetherItCan)
    {
        EXPECT_TRUE(controller.RequestsDepartureReleases());
    }

    TEST_F(ControllerPositionTest, ReceivesDepartureReleasesReturnsWhetherItCan)
    {
        EXPECT_FALSE(controller.ReceivesDepartureReleases());
    }

    TEST_F(ControllerPositionTest, ComparisonOperatorReturnsTrueIfSame)
    {
        ControllerPosition controller2(1, "EGFF_APP", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, false);
        EXPECT_TRUE(controller == controller2);
    }

    TEST_F(ControllerPositionTest, ComparisonOperatorReturnsTrueIfSameFrequencyDelta)
    {
        ControllerPosition controller2(1, "EGFF_APP", 125.8501, std::vector<std::string>{"EGGD", "EGFF"}, true, false);
        EXPECT_TRUE(controller == controller2);
    }

    TEST_F(ControllerPositionTest, ComparisonOperatorReturnsFalseIfDifferentCallsign)
    {
        ControllerPosition controller2(1, "EGFF_R_APP", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, false);
        EXPECT_FALSE(controller == controller2);
    }

    TEST_F(ControllerPositionTest, ComparisonOperatorReturnsFalseIfDifferentFrequency)
    {
        ControllerPosition controller2(1, "EGFF_APP", 125.650, std::vector<std::string>{"EGGD", "EGFF"}, true, false);
        EXPECT_FALSE(controller == controller2);
    }

    TEST_F(ControllerPositionTest, GetUnitReturnsTheAirfieldOrAreaUnit)
    {
        EXPECT_TRUE("EGFF" == controller.GetUnit());
    }

    TEST_F(ControllerPositionTest, ItReturnsTrueOnTopdownAirfield)
    {
        EXPECT_TRUE(controller.HasTopdownAirfield("EGFF"));
        EXPECT_TRUE(controller.HasTopdownAirfield("EGGD"));
    }

    TEST_F(ControllerPositionTest, ItReturnsFalseOnNoTopdownAirfield)
    {
        EXPECT_FALSE(controller.HasTopdownAirfield("EGLL"));
    }

    TEST_F(ControllerPositionTest, SendsPrenoteMessagesReturnsWhetherItCan)
    {
        EXPECT_FALSE(controller.SendsPrenoteMessages());
    }

    TEST_F(ControllerPositionTest, ReceivesPrenoteMessagesReturnsWhetherItCan)
    {
        EXPECT_TRUE(controller.ReceivesPrenoteMessages());
    }
} // namespace UKControllerPluginTest::Controller
