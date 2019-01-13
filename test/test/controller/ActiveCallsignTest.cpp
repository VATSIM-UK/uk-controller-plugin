#include "pch/pch.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPluginTest {
    namespace Controller {

        TEST(ActiveCallsign, LessThanOperatorReturnsTrueIfCallsignLess)
        {
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", { "EGKK" });
            ActiveCallsign pos1("LON_S1_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_TRUE(pos1 < pos2);
        }

        TEST(ActiveCallsign, LessThanOperatorReturnsFalseIfCallsignSame)
        {
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", { "EGKK" });
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_FALSE(pos1 < pos2);
        }

        TEST(ActiveCallsign, LessThanOperatorReturnsFalseIfCallsignMore)
        {
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", { "EGKK" });
            ActiveCallsign pos1("LON_S__CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_FALSE(pos1 < pos2);
        }

        TEST(ActiveCallsign, GreaterThanOperatorReturnsFalseIfCallsignLess)
        {
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", { "EGKK" });
            ActiveCallsign pos1("LON_S1_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_FALSE(pos1 > pos2);
        }

        TEST(ActiveCallsign, GreaterThanOperatorReturnsFalseIfCallsignSame)
        {
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", { "EGKK" });
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_FALSE(pos1 > pos2);
        }

        TEST(ActiveCallsign, GreaterThanOperatorReturnsTrueIfCallsignMore)
        {
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", { "EGKK" });
            ActiveCallsign pos1("LON_S__CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_TRUE(pos1 > pos2);
        }

        TEST(ActiveCallsign, EqualityOperatorReturnsTrueIfAllEqual)
        {
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", { "EGKK" });
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_TRUE(pos1 == pos2);
        }

        TEST(ActiveCallsign, EqualityOperatorReturnsFalseIfCallsignDifferent)
        {
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", { "EGKK" });
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S__CTR", "Testy McTest", controller);
            EXPECT_FALSE(pos1 == pos2);
        }

        TEST(ActiveCallsign, EqualityOperatorReturnsFalseIfControllerDifferent)
        {
            ControllerPosition controller1("LON_S_CTR", 129.420, "CTR", { "EGKK" });
            ControllerPosition controller2("LON_C_CTR", 127.100, "CTR", { "EGSS" });
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller1);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller2);
            EXPECT_FALSE(pos1 == pos2);
        }

        TEST(ActiveCallsign, InequalityOperatorReturnsFalseIfAllSame)
        {
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", { "EGKK" });
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_FALSE(pos1 != pos2);
        }

        TEST(ActiveCallsign, InequalityOperatorReturnsTrueIfCallsignNotSame)
        {
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", { "EGKK" });
            ActiveCallsign pos1("LON_S__CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_TRUE(pos1 != pos2);
        }

        TEST(ActiveCallsign, InequalityOperatorReturnsTrueIfControllerDifferent)
        {
            ControllerPosition controller1("LON_S_CTR", 129.420, "CTR", { "EGKK" });
            ControllerPosition controller2("LON_C_CTR", 127.100, "CTR", { "EGSS" });
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller1);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller2);
            EXPECT_TRUE(pos1 != pos2);
        }

        TEST(ActiveCallsign, CopyConstructorCopiesAllItems)
        {
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", { "EGKK" });
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2 = pos1;
            EXPECT_TRUE(pos1 == pos2);
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
