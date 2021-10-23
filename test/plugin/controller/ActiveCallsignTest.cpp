#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPluginTest::Controller {

    class ActiveCallsignTest : public testing::Test
    {
        public:
        ActiveCallsignTest()
            : controller(1, "LON_S_CTR", 129.420, {"EGKK"}, true, false),
              controller2(2, "LON_C_CTR", 127.100, {"EGSS"}, true, false)
        {
        }

        ControllerPosition controller;
        ControllerPosition controller2;
    };

    TEST_F(ActiveCallsignTest, ItReturnsCallsign)
    {
        ActiveCallsign pos1("LON_S1_CTR", "Testy McTest", controller, true);
        EXPECT_EQ("LON_S1_CTR", pos1.GetCallsign());
    }

    TEST_F(ActiveCallsignTest, ItReturnsControllerName)
    {
        ActiveCallsign pos1("LON_S1_CTR", "Testy McTest", controller, true);
        EXPECT_EQ("Testy McTest", pos1.GetControllerName());
    }

    TEST_F(ActiveCallsignTest, ItReturnsNormalisedPosition)
    {
        ActiveCallsign pos1("LON_S1_CTR", "Testy McTest", controller, true);
        EXPECT_EQ(controller, pos1.GetNormalisedPosition());
    }

    TEST_F(ActiveCallsignTest, ItReturnsIsUser)
    {
        ActiveCallsign pos1("LON_S1_CTR", "Testy McTest", controller, true);
        EXPECT_TRUE(pos1.GetIsUser());
    }

    TEST_F(ActiveCallsignTest, LessThanOperatorReturnsTrueIfCallsignLess)
    {
        ActiveCallsign pos1("LON_S1_CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller, true);
        EXPECT_TRUE(pos1 < pos2);
    }

    TEST_F(ActiveCallsignTest, LessThanOperatorReturnsFalseIfCallsignSame)
    {
        ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller, true);
        EXPECT_FALSE(pos1 < pos2);
    }

    TEST_F(ActiveCallsignTest, LessThanOperatorReturnsFalseIfCallsignMore)
    {
        ActiveCallsign pos1("LON_S__CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller, true);
        EXPECT_FALSE(pos1 < pos2);
    }

    TEST_F(ActiveCallsignTest, GreaterThanOperatorReturnsFalseIfCallsignLess)
    {
        ActiveCallsign pos1("LON_S1_CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller, true);
        EXPECT_FALSE(pos1 > pos2);
    }

    TEST_F(ActiveCallsignTest, GreaterThanOperatorReturnsFalseIfCallsignSame)
    {
        ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller, true);
        EXPECT_FALSE(pos1 > pos2);
    }

    TEST_F(ActiveCallsignTest, GreaterThanOperatorReturnsTrueIfCallsignMore)
    {
        ActiveCallsign pos1("LON_S__CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller, true);
        EXPECT_TRUE(pos1 > pos2);
    }

    TEST_F(ActiveCallsignTest, EqualityOperatorReturnsTrueIfAllEqual)
    {
        ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller, true);
        EXPECT_TRUE(pos1 == pos2);
    }

    TEST_F(ActiveCallsignTest, EqualityOperatorReturnsFalseIfCallsignDifferent)
    {
        ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S__CTR", "Testy McTest", controller, true);
        EXPECT_FALSE(pos1 == pos2);
    }

    TEST_F(ActiveCallsignTest, EqualityOperatorReturnsFalseIfControllerDifferent)
    {
        ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller2, true);
        EXPECT_FALSE(pos1 == pos2);
    }

    TEST_F(ActiveCallsignTest, EqualityOperatorReturnsFalseIfIsUserDifferent)
    {
        ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller, false);
        EXPECT_FALSE(pos1 == pos2);
    }

    TEST_F(ActiveCallsignTest, InequalityOperatorReturnsFalseIfAllSame)
    {
        ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller, true);
        EXPECT_FALSE(pos1 != pos2);
    }

    TEST_F(ActiveCallsignTest, InequalityOperatorReturnsTrueIfCallsignNotSame)
    {
        ActiveCallsign pos1("LON_S__CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller, true);
        EXPECT_TRUE(pos1 != pos2);
    }

    TEST_F(ActiveCallsignTest, InequalityOperatorReturnsTrueIfControllerDifferent)
    {
        ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller2, true);
        EXPECT_TRUE(pos1 != pos2);
    }

    TEST_F(ActiveCallsignTest, InequalityOperatorReturnsTrueIfIsUser)
    {
        ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2("LON_S_CTR", "Testy McTest", controller, false);
        EXPECT_TRUE(pos1 != pos2);
    }

    TEST_F(ActiveCallsignTest, CopyConstructorCopiesAllItems)
    {
        ActiveCallsign pos1("LON_S_CTR", "Testy McTest", controller, true);
        ActiveCallsign pos2 = pos1;
        EXPECT_TRUE(pos1 == pos2);
    }
} // namespace UKControllerPluginTest::Controller
