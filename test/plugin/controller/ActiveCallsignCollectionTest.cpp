#include "controller/ActiveCallsignCollection.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"

using ::testing::NiceMock;
using ::testing::Ref;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPluginTest::Controller::MockActiveCallsignEventHandler;

namespace UKControllerPluginTest {
    namespace Controller {

        class ActiveCallsignCollectionTest : public Test
        {
            public:
            ActiveCallsignCollectionTest()
                : testPosition(1, "LON_S_CTR", 129.420, {}, true, false),
                  testCallsign("LON_S_CTR", "Testy Boi", testPosition)
            {
                handler1.reset(new NiceMock<MockActiveCallsignEventHandler>());
                handler2.reset(new NiceMock<MockActiveCallsignEventHandler>());
            }

            std::shared_ptr<NiceMock<MockActiveCallsignEventHandler>> handler1;
            std::shared_ptr<NiceMock<MockActiveCallsignEventHandler>> handler2;
            ActiveCallsignCollection collection;
            ControllerPosition testPosition;
            ActiveCallsign testCallsign;
        };

        TEST_F(ActiveCallsignCollectionTest, ItStartsWithNoHandlers)
        {
            EXPECT_EQ(0, this->collection.CountHandlers());
        }

        TEST_F(ActiveCallsignCollectionTest, ItAddsHandlers)
        {
            this->collection.AddHandler(handler1);
            this->collection.AddHandler(handler2);
            EXPECT_EQ(2, this->collection.CountHandlers());
        }

        TEST_F(ActiveCallsignCollectionTest, ItDoesntAddDuplicateHandlers)
        {
            this->collection.AddHandler(handler1);
            this->collection.AddHandler(handler1);
            EXPECT_EQ(1, this->collection.CountHandlers());
        }

        TEST_F(ActiveCallsignCollectionTest, ItHandlesNewCallsignEvents)
        {
            this->collection.AddHandler(handler1);
            this->collection.AddHandler(handler2);

            EXPECT_CALL(*this->handler1, ActiveCallsignAdded(this->testCallsign, false)).Times(1);
            EXPECT_CALL(*this->handler2, ActiveCallsignAdded(this->testCallsign, false)).Times(1);

            this->collection.AddCallsign(this->testCallsign);
        }

        TEST_F(ActiveCallsignCollectionTest, ItHandlesNewUserCallsignEvents)
        {
            this->collection.AddHandler(handler1);
            this->collection.AddHandler(handler2);

            EXPECT_CALL(*this->handler1, ActiveCallsignAdded(this->testCallsign, true)).Times(1);
            EXPECT_CALL(*this->handler2, ActiveCallsignAdded(this->testCallsign, true)).Times(1);

            this->collection.AddUserCallsign(this->testCallsign);
        }

        TEST_F(ActiveCallsignCollectionTest, ItHandlesRemovedCallsignEvents)
        {
            this->collection.AddHandler(handler1);
            this->collection.AddHandler(handler2);

            EXPECT_CALL(*this->handler1, ActiveCallsignRemoved(this->testCallsign, false)).Times(1);
            EXPECT_CALL(*this->handler2, ActiveCallsignRemoved(this->testCallsign, false)).Times(1);

            EXPECT_CALL(*this->handler1, ActiveCallsignAdded(this->testCallsign, false)).Times(1);
            EXPECT_CALL(*this->handler2, ActiveCallsignAdded(this->testCallsign, false)).Times(1);

            this->collection.AddCallsign(this->testCallsign);
            this->collection.RemoveCallsign(this->testCallsign);
        }

        TEST_F(ActiveCallsignCollectionTest, ItHandlesRemovedUserCallsignEvents)
        {
            this->collection.AddHandler(handler1);
            this->collection.AddHandler(handler2);

            EXPECT_CALL(*this->handler1, ActiveCallsignRemoved(this->testCallsign, true)).Times(1);
            EXPECT_CALL(*this->handler2, ActiveCallsignRemoved(this->testCallsign, true)).Times(1);

            EXPECT_CALL(*this->handler1, ActiveCallsignAdded(this->testCallsign, true)).Times(1);
            EXPECT_CALL(*this->handler2, ActiveCallsignAdded(this->testCallsign, true)).Times(1);

            this->collection.AddUserCallsign(this->testCallsign);
            this->collection.RemoveCallsign(this->testCallsign);
        }

        TEST_F(ActiveCallsignCollectionTest, ItHandlesCallsignFlushEvents)
        {
            this->collection.AddHandler(handler1);
            this->collection.AddHandler(handler2);

            EXPECT_CALL(*this->handler1, CallsignsFlushed()).Times(1);
            EXPECT_CALL(*this->handler2, CallsignsFlushed()).Times(1);

            this->collection.Flush();
        }

        TEST_F(ActiveCallsignCollectionTest, AddCallsignThrowsExceptionIfAlreadyAdded)
        {
            EXPECT_NO_THROW(collection.AddCallsign(this->testCallsign));
            EXPECT_THROW(collection.AddCallsign(this->testCallsign), std::invalid_argument);
        }

        TEST_F(ActiveCallsignCollectionTest, PositionActiveDefaultsToFalse)
        {
            EXPECT_FALSE(collection.PositionActive("LON_S_CTR"));
        }

        TEST_F(ActiveCallsignCollectionTest, PositionActiveReturnsTrueIfCallsignActive)
        {
            collection.AddCallsign(this->testCallsign);
            EXPECT_TRUE(collection.PositionActive("LON_S_CTR"));
        }

        TEST_F(ActiveCallsignCollectionTest, PositionActiveReturnsFalseIfCallsignInactive)
        {
            collection.AddCallsign(this->testCallsign);
            EXPECT_TRUE(collection.CallsignActive("LON_S_CTR"));
            collection.RemoveCallsign(this->testCallsign);
            EXPECT_FALSE(collection.PositionActive("LON_S_CTR"));
        }

        TEST_F(ActiveCallsignCollectionTest, CallsignActiveReturnsTrueIfCallsignActive)
        {
            collection.AddCallsign(this->testCallsign);
            EXPECT_TRUE(collection.CallsignActive("LON_S_CTR"));
        }

        TEST_F(ActiveCallsignCollectionTest, CallsignActiveReturnsTrueIfCallsignInactive)
        {
            collection.AddCallsign(this->testCallsign);
            EXPECT_TRUE(collection.CallsignActive("LON_S_CTR"));
            collection.RemoveCallsign(this->testCallsign);
            EXPECT_FALSE(collection.CallsignActive("LON_S_CTR"));
        }

        TEST_F(ActiveCallsignCollectionTest, GetLeadCallsignForPositionThrowsExceptionIfNotFound)
        {
            EXPECT_THROW(collection.GetLeadCallsignForPosition("LON_S_CTR"), std::out_of_range);
        }

        TEST_F(ActiveCallsignCollectionTest, GetLeadCallsignForPositionKeepsItemsInOrderBasedOnCallsign)
        {
            ActiveCallsign cs1("LON_S1_CTR", "Bob Jones", this->testPosition);
            ActiveCallsign cs2("LON_S__CTR", "Jones Bob", this->testPosition);

            collection.AddCallsign(cs1);
            collection.AddCallsign(cs2);
            EXPECT_EQ(0, collection.GetLeadCallsignForPosition("LON_S_CTR").GetCallsign().compare("LON_S1_CTR"));
        }

        TEST_F(ActiveCallsignCollectionTest, GetLeadCallsignForPositionKeepsItemsInOrderIfAddedInWrongOrder)
        {
            ActiveCallsign cs1("LON_S1_CTR", "Bob Jones", this->testPosition);
            ActiveCallsign cs2("LON_S__CTR", "Jones Bob", this->testPosition);

            collection.AddCallsign(cs2);
            collection.AddCallsign(cs1);
            EXPECT_EQ(0, collection.GetLeadCallsignForPosition("LON_S_CTR").GetCallsign().compare("LON_S1_CTR"));
        }

        TEST_F(ActiveCallsignCollectionTest, GetCallsignThrowsExceptionIfNotFound)
        {
            EXPECT_THROW(collection.GetCallsign("LON_S_CTR"), std::out_of_range);
        }

        TEST_F(ActiveCallsignCollectionTest, GetCallsignReturnsCorrectItem)
        {
            collection.AddCallsign(this->testCallsign);
            EXPECT_EQ(this->testCallsign, collection.GetCallsign("LON_S_CTR"));
        }

        TEST_F(ActiveCallsignCollectionTest, GetUserCallsignThrowsExceptionIfNotFound)
        {
            EXPECT_THROW(collection.GetUserCallsign(), std::out_of_range);
        }

        TEST_F(ActiveCallsignCollectionTest, GetUserCallsignReturnsUserCallsign)
        {
            collection.AddUserCallsign(this->testCallsign);
            EXPECT_EQ(collection.GetUserCallsign(), this->testCallsign);
        }

        TEST_F(ActiveCallsignCollectionTest, AddingExtraUserCallsignRemovesTheFirst)
        {
            ActiveCallsign callsign("LON_S_CTR", "Testy McTest", this->testPosition);
            ActiveCallsign callsign2("LON_S1_CTR", "Testy McTest", this->testPosition);
            collection.AddUserCallsign(callsign);
            collection.AddUserCallsign(callsign2);
            EXPECT_EQ(collection.GetUserCallsign(), callsign2);
            EXPECT_FALSE(collection.CallsignActive("LON_S_CTR"));
        }

        TEST_F(ActiveCallsignCollectionTest, UserHasCallsignReturnsFalseIfNoneSet)
        {
            EXPECT_FALSE(collection.UserHasCallsign());
        }

        TEST_F(ActiveCallsignCollectionTest, UserHasCallsignReturnsTrueIfSet)
        {
            collection.AddUserCallsign(this->testCallsign);
            EXPECT_TRUE(collection.UserHasCallsign());
        }

        TEST_F(ActiveCallsignCollectionTest, FlushEmptiesTheCollection)
        {
            ControllerPosition controller1(1, "LON_S_CTR", 129.420, {}, true, false);
            ControllerPosition controller2(2, "LON_C_CTR", 127.100, {}, true, false);
            ControllerPosition controller3(3, "LON_E_CTR", 121.200, {}, true, false);
            ActiveCallsign cs1("LON_S_CTR", "Bob Jones", controller1);
            ActiveCallsign cs2("LON_C_CTR", "Jones Bob", controller2);
            ActiveCallsign cs3("LON_E_CTR", "Jimbob Jump", controller3);

            collection.AddCallsign(cs1);
            collection.AddCallsign(cs2);
            collection.AddCallsign(cs3);

            collection.Flush();
            EXPECT_FALSE(collection.CallsignActive("LON_S_CTR"));
            EXPECT_FALSE(collection.PositionActive("LON_S_CTR"));
            EXPECT_FALSE(collection.CallsignActive("LON_C_CTR"));
            EXPECT_FALSE(collection.PositionActive("LON_C_CTR"));
            EXPECT_FALSE(collection.CallsignActive("LON_E_CTR"));
            EXPECT_FALSE(collection.PositionActive("LON_E_CTR"));
        }

        TEST_F(ActiveCallsignCollectionTest, FlushMarksUserAsInactive)
        {
            collection.AddUserCallsign(this->testCallsign);
            EXPECT_TRUE(collection.UserHasCallsign());
            collection.Flush();
            EXPECT_FALSE(collection.UserHasCallsign());
        }

        TEST_F(ActiveCallsignCollectionTest, CollectionCanHandlePositionChanges)
        {
            collection.AddUserCallsign(this->testCallsign);
            EXPECT_TRUE(collection.UserHasCallsign());
            collection.Flush();
            EXPECT_FALSE(collection.UserHasCallsign());
            collection.AddUserCallsign(this->testCallsign);
            EXPECT_TRUE(collection.UserHasCallsign());
        }

        TEST_F(ActiveCallsignCollectionTest, RemoveCallsignSetsUserInactiveIfCurrentUser)
        {
            collection.AddUserCallsign(this->testCallsign);
            EXPECT_TRUE(collection.UserHasCallsign());
            collection.RemoveCallsign(this->testCallsign);
            EXPECT_FALSE(collection.UserHasCallsign());
        }

        TEST_F(ActiveCallsignCollectionTest, RemoveCallsignDoesNotChangeUserIfNotUserLoggingOff)
        {
            ControllerPosition pos2(2, "LON_E_CTR", 121.220, {}, true, false);
            ActiveCallsign callsign2("LON_E_CTR", "Testy McTestingon", pos2);
            collection.AddUserCallsign(this->testCallsign);
            collection.AddCallsign(callsign2);
            EXPECT_TRUE(collection.UserHasCallsign());
            collection.RemoveCallsign(callsign2);
            EXPECT_TRUE(collection.UserHasCallsign());
        }

        TEST_F(ActiveCallsignCollectionTest, CorrectlyHandlesMismatchingCallsigns)
        {
            ControllerPosition pos(1, "LON_S_CTR", 129.420, {"EGKK", "EGLL", "EGLC"}, true, false);
            ControllerPosition pos2(2, "LON_N_CTR", 133.700, {"EGCC", "EGGP"}, true, false);
            ActiveCallsign callsign("LON_S_CTR", "Testy McTest", pos2);
            ActiveCallsign callsign2("LON_S_CTR", "Testy McTest", pos2);
            collection.AddUserCallsign(callsign);
            EXPECT_TRUE(collection.PositionActive("LON_N_CTR"));
            EXPECT_TRUE(collection.CallsignActive("LON_S_CTR"));
            collection.RemoveCallsign(callsign);
            EXPECT_FALSE(collection.PositionActive("LON_N_CTR"));
            EXPECT_FALSE(collection.CallsignActive("LON_S_CTR"));
            collection.AddUserCallsign(callsign);
            EXPECT_TRUE(collection.PositionActive("LON_N_CTR"));
            EXPECT_TRUE(collection.CallsignActive("LON_S_CTR"));
            collection.RemoveCallsign(callsign);
            EXPECT_FALSE(collection.PositionActive("LON_N_CTR"));
            EXPECT_FALSE(collection.CallsignActive("LON_S_CTR"));
        }
    } // namespace Controller
} // namespace UKControllerPluginTest
