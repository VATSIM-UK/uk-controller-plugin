#include "pch/pch.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPluginTest {
    namespace Controller {

        TEST(ActiveCallsignCollection, AddCallsignThrowsExceptionIfAlreadyAdded)
        {
            ActiveCallsignCollection collection;
            ControllerPosition pos("LON_S_CTR", 129.420, "CTR", { "EGKK", "EGLL", "EGLC" });
            ActiveCallsign callsign("LON_S_CTR", "Testy McTest", pos);

            EXPECT_NO_THROW(collection.AddCallsign(callsign));
            EXPECT_THROW(collection.AddCallsign(callsign), std::invalid_argument);
        }

        TEST(ActiveCallsignCollection, PositionActiveDefaultsToFalse)
        {
            ActiveCallsignCollection collection;

            EXPECT_FALSE(collection.PositionActive("LON_S_CTR"));
        }

        TEST(ActiveCallsignCollection, PositionActiveReturnsTrueIfCallsignActive)
        {
            ActiveCallsignCollection collection;
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", {});
            ActiveCallsign cs("LON_S_CTR", "Bob Jones", controller);

            collection.AddCallsign(cs);
            EXPECT_TRUE(collection.PositionActive("LON_S_CTR"));
        }

        TEST(ActiveCallsignCollection, PositionActiveReturnsTrueIfCallsignInactive)
        {
            ActiveCallsignCollection collection;
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", {});
            ActiveCallsign cs("LON_S_CTR", "Bob Jones", controller);

            collection.AddCallsign(cs);
            EXPECT_TRUE(collection.CallsignActive("LON_S_CTR"));
            collection.RemoveCallsign(cs);
            EXPECT_FALSE(collection.PositionActive("LON_S_CTR"));
        }

        TEST(ActiveCallsignCollection, CallsignActiveReturnsTrueIfCallsignActive)
        {
            ActiveCallsignCollection collection;
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", {});
            ActiveCallsign cs("LON_S_CTR", "Bob Jones", controller);

            collection.AddCallsign(cs);
            EXPECT_TRUE(collection.CallsignActive("LON_S_CTR"));
        }

        TEST(ActiveCallsignCollection, CallsignActiveReturnsTrueIfCallsignInactive)
        {
            ActiveCallsignCollection collection;
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", {});
            ActiveCallsign cs("LON_S_CTR", "Bob Jones", controller);

            collection.AddCallsign(cs);
            EXPECT_TRUE(collection.CallsignActive("LON_S_CTR"));
            collection.RemoveCallsign(cs);
            EXPECT_FALSE(collection.CallsignActive("LON_S_CTR"));
        }

        TEST(ActiveCallsignCollection, GetLeadCallsignForPositionThrowsExceptionIfNotFound)
        {
            ActiveCallsignCollection collection;
            EXPECT_THROW(collection.GetLeadCallsignForPosition("LON_S_CTR"), std::out_of_range);
        }

        TEST(ActiveCallsignCollection, GetLeadCallsignForPositionKeepsItemsInOrderBasedOnCallsign)
        {
            ActiveCallsignCollection collection;
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", {});
            ActiveCallsign cs1("LON_S1_CTR", "Bob Jones", controller);
            ActiveCallsign cs2("LON_S__CTR", "Jones Bob", controller);

            collection.AddCallsign(cs1);
            collection.AddCallsign(cs2);
            EXPECT_EQ(0, collection.GetLeadCallsignForPosition("LON_S_CTR").GetCallsign().compare("LON_S1_CTR"));
        }

        TEST(ActiveCallsignCollection, GetLeadCallsignForPositionKeepsItemsInOrderIfAddedInWrongOrder)
        {
            ActiveCallsignCollection collection;
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", {});
            ActiveCallsign cs1("LON_S1_CTR", "Bob Jones", controller);
            ActiveCallsign cs2("LON_S__CTR", "Jones Bob", controller);

            collection.AddCallsign(cs2);
            collection.AddCallsign(cs1);
            EXPECT_EQ(0, collection.GetLeadCallsignForPosition("LON_S_CTR").GetCallsign().compare("LON_S1_CTR"));
        }

        TEST(ActiveCallsignCollection, GetCallsignThrowsExceptionIfNotFound)
        {
            ActiveCallsignCollection collection;
            EXPECT_THROW(collection.GetCallsign("LON_S_CTR"), std::out_of_range);
        }

        TEST(ActiveCallsignCollection, GetCallsignReturnsCorrectItem)
        {
            ActiveCallsignCollection collection;
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", {});
            ActiveCallsign cs1("LON_S1_CTR", "Bob Jones", controller);

            collection.AddCallsign(cs1);
            EXPECT_EQ(cs1, collection.GetCallsign("LON_S1_CTR"));
        }

        TEST(ActiveCallsignCollection, GetUserCallsignThrowsExceptionIfNotFound)
        {
            ActiveCallsignCollection collection;
            EXPECT_THROW(collection.GetUserCallsign(), std::out_of_range);
        }

        TEST(ActiveCallsignCollection, GetUserCallsignReturnsUserCallsign)
        {
            ActiveCallsignCollection collection;
            ControllerPosition pos("LON_S_CTR", 129.420, "CTR", { "EGKK", "EGLL", "EGLC" });
            ActiveCallsign callsign("LON_S_CTR", "Testy McTest", pos);
            collection.AddUserCallsign(callsign);
            EXPECT_EQ(collection.GetUserCallsign(), callsign);
        }

        TEST(ActiveCallsignCollection, UserHasCallsignReturnsFalseIfNoneSet)
        {
            ActiveCallsignCollection collection;
            EXPECT_FALSE(collection.UserHasCallsign());
        }

        TEST(ActiveCallsignCollection, UserHasCallsignReturnsTrueIfSet)
        {
            ActiveCallsignCollection collection;
            ControllerPosition pos("LON_S_CTR", 129.420, "CTR", { "EGKK", "EGLL", "EGLC" });
            ActiveCallsign callsign("LON_S_CTR", "Testy McTest", pos);
            collection.AddUserCallsign(callsign);
            EXPECT_TRUE(collection.UserHasCallsign());
        }

        TEST(ActiveCallsignCollection, FlushEmptiesTheCollection)
        {
            ActiveCallsignCollection collection;
            ControllerPosition controller1("LON_S_CTR", 129.420, "CTR", {});
            ControllerPosition controller2("LON_C_CTR", 127.100, "CTR", {});
            ControllerPosition controller3("LON_E_CTR", 121.200, "CTR", {});
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

        TEST(ActiveCallsignCollection, FlushMarksUserAsInactive)
        {
            ActiveCallsignCollection collection;
            ControllerPosition pos("LON_S_CTR", 129.420, "CTR", { "EGKK", "EGLL", "EGLC" });
            ActiveCallsign callsign("LON_S_CTR", "Testy McTest", pos);
            collection.AddUserCallsign(callsign);
            EXPECT_TRUE(collection.UserHasCallsign());
            collection.Flush();
            EXPECT_FALSE(collection.UserHasCallsign());
        }

        TEST(ActiveCallsignCollection, CollectionCanHandlePositionChanges)
        {
            ActiveCallsignCollection collection;
            ControllerPosition pos("LON_S_CTR", 129.420, "CTR", { "EGKK", "EGLL", "EGLC" });
            ActiveCallsign callsign("LON_S_CTR", "Testy McTest", pos);
            collection.AddUserCallsign(callsign);
            EXPECT_TRUE(collection.UserHasCallsign());
            collection.Flush();
            EXPECT_FALSE(collection.UserHasCallsign());
            collection.AddUserCallsign(callsign);
            EXPECT_TRUE(collection.UserHasCallsign());
        }

        TEST(ActiveCallsignCollection, RemoveCallsignSetsUserInactiveIfCurrentUser)
        {
            ActiveCallsignCollection collection;
            ControllerPosition pos("LON_S_CTR", 129.420, "CTR", { "EGKK", "EGLL", "EGLC" });
            ActiveCallsign callsign("LON_S_CTR", "Testy McTest", pos);
            collection.AddUserCallsign(callsign);
            EXPECT_TRUE(collection.UserHasCallsign());
            collection.RemoveCallsign(callsign);
            EXPECT_FALSE(collection.UserHasCallsign());
        }

        TEST(ActiveCallsignCollection, RemoveCallsignDoesNotChangeUserIfNotUserLoggingOff)
        {
            ActiveCallsignCollection collection;
            ControllerPosition pos("LON_S_CTR", 129.420, "CTR", { "EGKK", "EGLL", "EGLC" });
            ControllerPosition pos2("LON_E_CTR", 121.220, "CTR", { });
            ActiveCallsign callsign("LON_S_CTR", "Testy McTest", pos);
            ActiveCallsign callsign2("LON_E_CTR", "Testy McTestingon", pos2);
            collection.AddUserCallsign(callsign);
            collection.AddCallsign(callsign2);
            EXPECT_TRUE(collection.UserHasCallsign());
            collection.RemoveCallsign(callsign2);
            EXPECT_TRUE(collection.UserHasCallsign());
        }

        TEST(ActiveCallsignCollection, CorrectlyHandlesMismatchingCallsigns)
        {
            ActiveCallsignCollection collection;
            ControllerPosition pos("LON_S_CTR", 129.420, "CTR", { "EGKK", "EGLL", "EGLC" });
            ControllerPosition pos2("LON_N_CTR", 133.700, "CTR", {"EGCC", "EGGP"});
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
    }  // namespace Controller
}  // namespace UKControllerPluginTest
