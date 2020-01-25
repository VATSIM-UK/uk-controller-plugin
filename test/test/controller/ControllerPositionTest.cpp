#include "pch/pch.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPluginTest {
    namespace Controller {

        TEST(ControllerPosition, GetCallsignReturnsCallsign)
        {
            ControllerPosition controller("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD, EGFF"});
            EXPECT_EQ(0, controller.GetCallsign().compare("EGFF_APP"));
        }

        TEST(ControllerPosition, GetFrequencyReturnsFrequency)
        {
            ControllerPosition controller("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD, EGFF"});
            EXPECT_EQ(125.850, controller.GetFrequency());
        }

        TEST(ControllerPosition, GetTypeReturnsType)
        {
            ControllerPosition controller("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD, EGFF"});
            EXPECT_EQ(0, controller.GetType().compare("APP"));
        }

        TEST(ControllerPosition, GetTopdownReturnsTopdown)
        {
            ControllerPosition controller("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            std::vector<std::string> expected = { "EGGD", "EGFF" };
            EXPECT_EQ(expected, controller.GetTopdown());
        }

        TEST(ControllerPosition, ComparisonOperatorReturnsTrueIfSame)
        {
            ControllerPosition controller1("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            ControllerPosition controller2("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            EXPECT_TRUE(controller1 == controller2);
        }

        TEST(ControllerPosition, ComparisonOperatorReturnsTrueIfSameFrequencyDelta)
        {
            ControllerPosition controller1("EGFF_APP", 125.8502, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            ControllerPosition controller2("EGFF_APP", 125.8501, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            EXPECT_TRUE(controller1 == controller2);
        }

        TEST(ControllerPosition, ComparisonOperatorReturnsFalseIfDifferentCallsign)
        {
            ControllerPosition controller1("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            ControllerPosition controller2("EGFF_R_APP", 125.850, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            EXPECT_FALSE(controller1 == controller2);
        }

        TEST(ControllerPosition, ComparisonOperatorReturnsFalseIfDifferentFrequency)
        {
            ControllerPosition controller1("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            ControllerPosition controller2("EGFF_APP", 125.650, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            EXPECT_FALSE(controller1 == controller2);
        }

        TEST(ControllerPosition, ComparisonOperatorReturnsFalseIfDifferentBoundary)
        {
            ControllerPosition controller1("EGFF_APP", 125.851, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            ControllerPosition controller2("EGFF_APP", 125.650, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            EXPECT_FALSE(controller1 == controller2);
        }

        TEST(ControllerPosition, ComparisonOperatorReturnsFalseIfDifferentType)
        {
            ControllerPosition controller1("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            ControllerPosition controller2("EGFF_APP", 125.850, "CTR", std::vector<std::string> {"EGGD", "EGFF"});
            EXPECT_FALSE(controller1 == controller2);
        }

        TEST(ControllerPosition, GetUnitReturnsTheAirfieldOrAreaUnit)
        {
            ControllerPosition controller("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            EXPECT_TRUE("EGFF" == controller.GetUnit());
        }

        TEST(ControllerPosition, ItReturnsTrueOnTopdownAirfield)
        {
            ControllerPosition controller("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            EXPECT_TRUE(controller.HasTopdownAirfield("EGFF"));
            EXPECT_TRUE(controller.HasTopdownAirfield("EGGD"));
        }

        TEST(ControllerPosition, ItReturnsFalseOnNoTopdownAirfield)
        {
            ControllerPosition controller("EGFF_APP", 125.850, "APP", std::vector<std::string> {"EGGD", "EGFF"});
            EXPECT_FALSE(controller.HasTopdownAirfield("EGLL"));
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
