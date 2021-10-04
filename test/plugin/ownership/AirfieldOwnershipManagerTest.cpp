#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"

using ::testing::Test;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;

namespace UKControllerPluginTest::Ownership {

    class AirfieldOwnershipManagerTest : public Test
    {
        public:
        AirfieldOwnershipManagerTest()
            : providers(std::make_shared<AirfieldServiceProviderCollection>()),
              controller1(1, "EGGD_TWR", 133.850, {"EGGD"}, true, true),
              controller2(2, "EGGD_GND", 121.920, {"EGGD"}, true, true),
              manager(providers, this->airfields, this->activeCallsigns)
        {
        }

        void SetUp() override
        {
            std::vector<std::string> topDown = {"EGGD_GND", "EGGD_TWR", "EGGD_GND", "EGFF_APP", "LON_W_CTR"};
            this->airfields.AddAirfield(std::make_unique<AirfieldModel>("EGGD", topDown));
        };

        std::shared_ptr<AirfieldServiceProviderCollection> providers;
        ControllerPosition controller1;
        ControllerPosition controller2;
        AirfieldOwnershipManager manager;
        ActiveCallsignCollection activeCallsigns;
        AirfieldCollection airfields;
    };

    TEST_F(AirfieldOwnershipManagerTest, AirfieldsStartWithNoDeliveryProvider)
    {
        EXPECT_FALSE(this->providers->AirfieldHasDeliveryProvider("EGGD"));
        EXPECT_TRUE(this->providers->GetServiceProviders("EGGD").empty());
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsDeliveryProvider)
    {
        this->activeCallsigns.AddCallsign(ActiveCallsign("EGGD_TWR", "Testy McTestface", controller1, false));
        this->manager.RefreshOwner("EGGD");

        EXPECT_TRUE(this->providers->AirfieldHasDeliveryProvider("EGGD"));
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_TWR"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsDeliveryProviderWithCorrectPriority)
    {
        ActiveCallsign active1("EGGD_TWR", "Testy McTestface", controller1, false);
        ActiveCallsign active2("EGGD_GND", "Testy McTestface 2", controller2, false);

        this->activeCallsigns.AddCallsign(active1);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_TWR"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);

        this->activeCallsigns.AddCallsign(active2);
        this->manager.RefreshOwner("EGGD");

        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_GND"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshAfterLogOffChangesDeliveryProvider)
    {
        ActiveCallsign active1("EGGD_TWR", "Testy McTestface", controller1, false);
        ActiveCallsign active2("EGGD_GND", "Testy McTestface 2", controller2, false);

        this->activeCallsigns.AddCallsign(active1);
        this->activeCallsigns.AddCallsign(active2);

        this->manager.RefreshOwner("EGGD");

        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_GND"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);

        this->activeCallsigns.RemoveCallsign(active2);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_TWR"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, FlushRemovesAllProviders)
    {
        ActiveCallsign active1("EGGD_TWR", "Testy McTestface", controller1, false);
        ActiveCallsign active2("EGGD_GND", "Testy McTestface 2", controller2, false);

        this->activeCallsigns.AddCallsign(active1);
        this->activeCallsigns.AddCallsign(active2);

        this->manager.RefreshOwner("EGGD");
        this->manager.Flush();

        EXPECT_TRUE(this->providers->GetServiceProviders("EGGD").empty());
    }
} // namespace UKControllerPluginTest::Ownership
