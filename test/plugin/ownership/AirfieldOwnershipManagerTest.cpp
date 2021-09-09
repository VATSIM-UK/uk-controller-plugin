#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "ownership/AirfieldOwnershipManager.h"

using ::testing::Test;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;

namespace UKControllerPluginTest::Ownership {

    class AirfieldOwnershipManagerTest : public Test
    {
        public:
        AirfieldOwnershipManagerTest()
            : controller1(1, "EGGD_TWR", 133.850, {"EGGD"}, true, true),
              controller2(2, "EGGD_GND", 121.920, {"EGGD"}, true, true), manager(this->airfields, this->activeCallsigns)
        {
        }

        void SetUp() override
        {
            std::vector<std::string> topDown = {"EGGD_GND", "EGGD_TWR", "EGGD_GND", "EGFF_APP", "LON_W_CTR"};
            this->airfields.AddAirfield(std::make_unique<AirfieldModel>("EGGD", topDown));
        };

        ControllerPosition controller1;
        ControllerPosition controller2;
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
        this->activeCallsigns.AddCallsign(ActiveCallsign("EGGD_TWR", "Testy McTestface", controller1));

        this->manager.RefreshOwner("EGGD");

        EXPECT_TRUE(this->manager.AirfieldHasOwner("EGGD"));
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsOwnerToCorrectcontroller1)
    {
        ActiveCallsign active("EGGD_TWR", "Testy McTestface", controller1);
        this->activeCallsigns.AddCallsign(active);

        this->manager.RefreshOwner("EGGD");

        EXPECT_TRUE(this->manager.AirfieldOwnedBy("EGGD", active));
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsOwnerWithCorrectPriority)
    {
        ActiveCallsign active1("EGGD_TWR", "Testy McTestface", controller1);
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
        ActiveCallsign active1("EGGD_TWR", "Testy McTestface", controller1);
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
        ActiveCallsign active1("EGGD_TWR", "Testy McTestface", controller1);
        ActiveCallsign active2("EGGD_GND", "Testy McTestface 2", controller2);

        this->activeCallsigns.AddCallsign(active1);
        this->activeCallsigns.AddCallsign(active2);

        this->manager.RefreshOwner("EGGD");
        this->manager.Flush();

        EXPECT_FALSE(this->manager.AirfieldHasOwner("EGGD"));
    }

    TEST_F(AirfieldOwnershipManagerTest, AirfieldOwnedByUserReturnsTrueIfActiveCallsignIsUser)
    {
        ActiveCallsign active("EGGD_TWR", "Testy McTestface", controller1);

        this->activeCallsigns.AddUserCallsign(active);

        this->manager.RefreshOwner("EGGD");

        EXPECT_TRUE(this->manager.AirfieldOwnedByUser("EGGD"));
    }

    TEST_F(AirfieldOwnershipManagerTest, AirfieldOwnedByUserReturnsFalseIfActiveCallsignNotUser)
    {
        ActiveCallsign active("EGGD_TWR", "Testy McTestface", controller1);

        this->activeCallsigns.AddCallsign(active);

        this->manager.RefreshOwner("EGGD");

        EXPECT_FALSE(this->manager.AirfieldOwnedByUser("EGGD"));
    }

    TEST_F(AirfieldOwnershipManagerTest, GetOwnerReturnsOwnerIfFound)
    {
        ActiveCallsign active("EGGD_TWR", "Testy McTestface", controller1);
        this->activeCallsigns.AddCallsign(active);

        this->manager.RefreshOwner("EGGD");

        EXPECT_EQ(active, this->manager.GetOwner("EGGD"));
    }

    TEST_F(AirfieldOwnershipManagerTest, GetOwnerReturnsNoneIfNotFound)
    {
        ActiveCallsign active("EGGD_TWR", "Testy McTestface", controller1);
        this->activeCallsigns.AddCallsign(active);

        EXPECT_EQ(this->manager.NotFoundCallsign(), this->manager.GetOwner("EGPF"));
    }

    TEST_F(AirfieldOwnershipManagerTest, GetOwnedReturnsNothingIfNotFound)
    {
        ActiveCallsign active1("EGGD_TWR", "Testy McTestface", controller1);
        ActiveCallsign active2("EGGD_GND", "Testy McTestface 2", controller2);
        this->activeCallsigns.AddCallsign(active1);
        this->activeCallsigns.AddCallsign(active2);
        this->manager.RefreshOwner("EGGD");

        EXPECT_EQ(0, this->manager.GetOwnedAirfields("EGGD_TWR").size());
    }

    TEST_F(AirfieldOwnershipManagerTest, GetOwnedReturnsOwnedAirfields)
    {
        ActiveCallsign active1("EGGD_TWR", "Testy McTestface", controller1);
        ActiveCallsign active2("EGGD_GND", "Testy McTestface 2", controller2);
        this->activeCallsigns.AddCallsign(active1);
        this->activeCallsigns.AddCallsign(active2);
        this->manager.RefreshOwner("EGGD");

        EXPECT_EQ(1, this->manager.GetOwnedAirfields("EGGD_GND").size());
        EXPECT_TRUE("EGGD" == this->manager.GetOwnedAirfields("EGGD_GND").begin()->get().GetIcao());
    }

    TEST_F(AirfieldOwnershipManagerTest, ItHasANoOwnerObject)
    {
        EXPECT_TRUE("" == this->manager.NotFoundCallsign().GetCallsign());
        EXPECT_TRUE("" == this->manager.NotFoundCallsign().GetControllerName());
    }
} // namespace UKControllerPluginTest::Ownership
