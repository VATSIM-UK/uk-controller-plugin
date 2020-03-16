#include "pch/pch.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "controller/ActiveCallsignCollection.h"
#include "airfield/AirfieldCollection.h"
#include "controller/ActiveCallsign.h"
#include "airfield/AirfieldModel.h"
#include "controller/ControllerPosition.h"

using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ControllerPosition;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Ownership {

        class AirfieldOwnershipManagerTest : public Test
        {
            public:

                AirfieldOwnershipManagerTest(void)
                    : manager(this->airfields, this->activeCallsigns)
                {

                }

                void SetUp()
                {
                    std::vector<std::string> topDown = { "EGGD_GND", "EGGD_TWR", "EGGD_APP", "EGFF_APP", "LON_W_CTR" };
                    this->airfields.AddAirfield(std::make_unique<UKControllerPlugin::Airfield::AirfieldModel>(
                        "EGGD",
                        topDown
                    ));
                };

                AirfieldOwnershipManager manager;
                ActiveCallsignCollection activeCallsigns;
                AirfieldCollection airfields;
        };

        TEST_F(AirfieldOwnershipManagerTest, AirfieldsStartWithNoOwner)
        {
            EXPECT_FALSE(this->manager.AirfieldHasOwner("EGGD"));
        }

        TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsOwner)
        {
            ControllerPosition controller("EGGD_TWR", 133.850, "TWR", { "EGGD" });
            this->activeCallsigns.AddCallsign(ActiveCallsign("EGGD_TWR", "Testy McTestface", controller));

            this->manager.RefreshOwner("EGGD");

            EXPECT_TRUE(this->manager.AirfieldHasOwner("EGGD"));
        }

        TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsOwnerToCorrectController)
        {
            ControllerPosition controller("EGGD_TWR", 133.850, "TWR", { "EGGD" });
            ActiveCallsign active("EGGD_TWR", "Testy McTestface", controller);
            this->activeCallsigns.AddCallsign(active);

            this->manager.RefreshOwner("EGGD");

            EXPECT_TRUE(this->manager.AirfieldOwnedBy("EGGD", active));
        }

        TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsOwnerWithCorrectPriority)
        {
            ControllerPosition controller1("EGGD_TWR", 133.850, "TWR", { "EGGD" });
            ActiveCallsign active1("EGGD_TWR", "Testy McTestface", controller1);

            ControllerPosition controller2("EGGD_GND", 121.920, "GND", { "EGGD" });
            ActiveCallsign active2("EGGD_GND", "Testy McTestface 2", controller2);

            this->activeCallsigns.AddCallsign(active1);
            this->manager.RefreshOwner("EGGD");
            EXPECT_TRUE(this->manager.AirfieldOwnedBy("EGGD", active1));

            this->activeCallsigns.AddCallsign(active2);
            this->manager.RefreshOwner("EGGD");

            EXPECT_TRUE(this->manager.AirfieldOwnedBy("EGGD", active2));
        }

        TEST_F(AirfieldOwnershipManagerTest, RefreshAfterLogOffChangesOwner)
        {
            ControllerPosition controller1("EGGD_TWR", 133.850, "TWR", { "EGGD" });
            ActiveCallsign active1("EGGD_TWR", "Testy McTestface", controller1);

            ControllerPosition controller2("EGGD_GND", 121.920, "GND", { "EGGD" });
            ActiveCallsign active2("EGGD_GND", "Testy McTestface 2", controller2);

            this->activeCallsigns.AddCallsign(active1);
            this->activeCallsigns.AddCallsign(active2);

            this->manager.RefreshOwner("EGGD");

            EXPECT_TRUE(this->manager.AirfieldOwnedBy("EGGD", active2));

            this->activeCallsigns.RemoveCallsign(active2);
            this->manager.RefreshOwner("EGGD");
            EXPECT_TRUE(this->manager.AirfieldOwnedBy("EGGD", active1));
        }

        TEST_F(AirfieldOwnershipManagerTest, FlushRemovesAllOwners)
        {
            ControllerPosition controller1("EGGD_TWR", 133.850, "TWR", { "EGGD" });
            ActiveCallsign active1("EGGD_TWR", "Testy McTestface", controller1);

            ControllerPosition controller2("EGGD_GND", 121.920, "GND", { "EGGD" });
            ActiveCallsign active2("EGGD_GND", "Testy McTestface 2", controller2);

            this->activeCallsigns.AddCallsign(active1);
            this->activeCallsigns.AddCallsign(active2);

            this->manager.RefreshOwner("EGGD");
            this->manager.Flush();

            EXPECT_FALSE(this->manager.AirfieldHasOwner("EGGD"));
        }

        TEST_F(AirfieldOwnershipManagerTest, AirfieldOwnedByUserReturnsTrueIfActiveCallsignIsUser)
        {
            ControllerPosition controller("EGGD_TWR", 133.850, "TWR", { "EGGD" });
            ActiveCallsign active("EGGD_TWR", "Testy McTestface", controller);

            this->activeCallsigns.AddUserCallsign(active);

            this->manager.RefreshOwner("EGGD");

            EXPECT_TRUE(this->manager.AirfieldOwnedByUser("EGGD"));
        }

        TEST_F(AirfieldOwnershipManagerTest, AirfieldOwnedByUserReturnsFalseIfActiveCallsignNotUser)
        {
            ControllerPosition controller("EGGD_TWR", 133.850, "TWR", { "EGGD" });
            ActiveCallsign active("EGGD_TWR", "Testy McTestface", controller);

            this->activeCallsigns.AddCallsign(active);

            this->manager.RefreshOwner("EGGD");

            EXPECT_FALSE(this->manager.AirfieldOwnedByUser("EGGD"));
        }

        TEST_F(AirfieldOwnershipManagerTest, GetOwnerReturnsOwnerIfFound)
        {
            ControllerPosition controller("EGGD_TWR", 133.850, "TWR", { "EGGD" });
            ActiveCallsign active("EGGD_TWR", "Testy McTestface", controller);
            this->activeCallsigns.AddCallsign(active);

            this->manager.RefreshOwner("EGGD");

            EXPECT_EQ(active, this->manager.GetOwner("EGGD"));
        }

        TEST_F(AirfieldOwnershipManagerTest, GetOwnerReturnsNoneIfNotFound)
        {
            ControllerPosition controller("EGGD_TWR", 133.850, "TWR", { "EGGD" });
            ActiveCallsign active("EGGD_TWR", "Testy McTestface", controller);
            this->activeCallsigns.AddCallsign(active);

            EXPECT_EQ(this->manager.notFoundCallsign, this->manager.GetOwner("EGPF"));
        }

        TEST_F(AirfieldOwnershipManagerTest, GetOwnedReturnsNothingIfNotFound)
        {
            ControllerPosition controller1("EGGD_TWR", 133.850, "TWR", { "EGGD" });
            ControllerPosition controller2("EGGD_APP", 133.850, "TWR", { "EGGD" });
            ActiveCallsign active1("EGGD_TWR", "Testy McTestface", controller1);
            ActiveCallsign active2("EGGD_APP", "Testy McTestface 2", controller2);
            this->activeCallsigns.AddCallsign(active1);
            this->activeCallsigns.AddCallsign(active2);
            this->manager.RefreshOwner("EGGD");

            EXPECT_EQ(0, this->manager.GetOwnedAirfields("EGGD_APP").size());
        }

        TEST_F(AirfieldOwnershipManagerTest, GetOwnedReturnsOwnedAirfields)
        {
            ControllerPosition controller1("EGGD_TWR", 133.850, "TWR", { "EGGD" });
            ControllerPosition controller2("EGGD_APP", 133.850, "TWR", { "EGGD" });
            ActiveCallsign active1("EGGD_TWR", "Testy McTestface", controller1);
            ActiveCallsign active2("EGGD_APP", "Testy McTestface 2", controller2);
            this->activeCallsigns.AddCallsign(active1);
            this->activeCallsigns.AddCallsign(active2);
            this->manager.RefreshOwner("EGGD");

            EXPECT_EQ(1, this->manager.GetOwnedAirfields("EGGD_TWR").size());
            EXPECT_TRUE("EGGD" == this->manager.GetOwnedAirfields("EGGD_TWR").begin()->GetIcao());
        }

        TEST_F(AirfieldOwnershipManagerTest, ItHasANoOwnerObject)
        {
            EXPECT_TRUE("" == this->manager.notFoundCallsign.GetCallsign());
            EXPECT_TRUE("" == this->manager.notFoundCallsign.GetControllerName());
        }
    }  // namespace Ownership
}  // namespace UKControllerPluginTest
