#include "pch/pch.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPluginTest {
    namespace Controller {

        class ActiveCallsignTest : public testing::Test
        {
            public:
                ActiveCallsignTest()
                    : controller(1, "LON_S_CTR", 129.420, {"EGKK"}, true, false),
                      controller2(2, "LON_C_CTR", 127.100, {"EGSS"}, true, false)
                { }

                ControllerPosition controller;
                ControllerPosition controller2;
        };

        TEST_F(ActiveCallsignTest, LessThanOperatorReturnsTrueIfCallsignLess)
        {
            ActiveCallsign pos1("LON_S1_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_TRUE(pos1 < pos2);
        }

        TEST_F(ActiveCallsignTest, LessThanOperatorReturnsFalseIfCallsignSame)
        {
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_FALSE(pos1 < pos2);
        }

        TEST_F(ActiveCallsignTest, LessThanOperatorReturnsFalseIfCallsignMore)
        {
            ActiveCallsign pos1("LON_S__CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_FALSE(pos1 < pos2);
        }

        TEST_F(ActiveCallsignTest, GreaterThanOperatorReturnsFalseIfCallsignLess)
        {
            ActiveCallsign pos1("LON_S1_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_FALSE(pos1 > pos2);
        }

        TEST_F(ActiveCallsignTest, GreaterThanOperatorReturnsFalseIfCallsignSame)
        {
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_FALSE(pos1 > pos2);
        }

        TEST_F(ActiveCallsignTest, GreaterThanOperatorReturnsTrueIfCallsignMore)
        {
            ActiveCallsign pos1("LON_S__CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_TRUE(pos1 > pos2);
        }

        TEST_F(ActiveCallsignTest, EqualityOperatorReturnsTrueIfAllEqual)
        {
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_TRUE(pos1 == pos2);
        }

        TEST_F(ActiveCallsignTest, EqualityOperatorReturnsFalseIfCallsignDifferent)
        {
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S__CTR", "Testy McTest", controller);
            EXPECT_FALSE(pos1 == pos2);
        }

        TEST_F(ActiveCallsignTest, EqualityOperatorReturnsFalseIfControllerDifferent)
        {
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller2);
            EXPECT_FALSE(pos1 == pos2);
        }

        TEST_F(ActiveCallsignTest, InequalityOperatorReturnsFalseIfAllSame)
        {
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_FALSE(pos1 != pos2);
        }

        TEST_F(ActiveCallsignTest, InequalityOperatorReturnsTrueIfCallsignNotSame)
        {
            ActiveCallsign pos1("LON_S__CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller);
            EXPECT_TRUE(pos1 != pos2);
        }

        TEST_F(ActiveCallsignTest, InequalityOperatorReturnsTrueIfControllerDifferent)
        {
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller2);
            EXPECT_TRUE(pos1 != pos2);
        }

        TEST_F(ActiveCallsignTest, CopyConstructorCopiesAllItems)
        {
            ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller);
            ActiveCallsign pos2 = pos1;
            EXPECT_TRUE(pos1 == pos2);
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
