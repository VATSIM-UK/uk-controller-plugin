#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
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
using UKControllerPlugin::Ownership::ServiceProvision;
using UKControllerPlugin::Ownership::ServiceType;

namespace UKControllerPluginTest::Ownership {

    class AirfieldOwnershipManagerTest : public Test
    {
        public:
        AirfieldOwnershipManagerTest()
            : providers(std::make_shared<AirfieldServiceProviderCollection>()),
              towerController(1, "EGGD_TWR", 133.850, {"EGGD"}, true, true),
              towerController2(8, "EGGD_2_TWR", 133.850, {"EGGD"}, true, true),
              groundController(2, "EGGD_GND", 121.920, {"EGGD"}, true, true),
              groundController2(9, "EGGD_2_GND", 121.920, {"EGGD"}, true, true),
              finalApproachController(3, "EGGD_F_APP", 121.920, {"EGGD"}, true, true),
              approachController(4, "EGGD_APP", 121.920, {"EGGD"}, true, true),
              enrouteController(5, "LON_W_CTR", 121.920, {"EGGD"}, true, true),
              enrouteController2(10, "LON_CTR", 121.920, {"EGGD"}, true, true),
              deliveryController(7, "EGGD_DEL", 121.920, {"EGGD"}, true, true),
              towerCallsign("EGGD_TWR", "Testy McTestface", towerController, false),
              towerCallsign2("EGGD_2_TWR", "Testy McTestface", towerController2, false),
              groundCallsign("EGGD_GND", "Testy McTestface", groundController, false),
              groundCallsign2("EGGD_2_GND", "Testy McTestface", groundController2, false),
              finalApproachCallsign("EGGD_F_APP", "Testy McTestface", finalApproachController, false),
              approachCallsign("EGGD_APP", "Testy McTestface", approachController, false),
              enrouteCallsign("LON_W_CTR", "Testy McTestface", enrouteController, false),
              enrouteCallsign2("LON_CTR", "Testy McTestface", enrouteController2, false),
              deliveryCallsign("EGGD_DEL", "Testy McTestface", deliveryController, false),
              manager(providers, this->airfields, this->activeCallsigns)
        {
        }

        void SetUp() override
        {
            std::vector<std::string> topDown = {
                "EGGD_DEL",
                "EGGD_GND",
                "EGGD_2_GND",
                "EGGD_TWR",
                "EGGD_2_TWR",
                "EGGD_F_APP",
                "EGGD_APP",
                "LON_W_CTR",
                "LON_CTR"};
            this->airfields.AddAirfield(std::make_unique<AirfieldModel>("EGGD", topDown));
        };

        std::shared_ptr<AirfieldServiceProviderCollection> providers;
        ControllerPosition towerController;
        ControllerPosition towerController2;
        ControllerPosition groundController;
        ControllerPosition groundController2;
        ControllerPosition finalApproachController;
        ControllerPosition approachController;
        ControllerPosition enrouteController;
        ControllerPosition enrouteController2;
        ControllerPosition deliveryController;
        ActiveCallsign towerCallsign;
        ActiveCallsign towerCallsign2;
        ActiveCallsign groundCallsign;
        ActiveCallsign groundCallsign2;
        ActiveCallsign finalApproachCallsign;
        ActiveCallsign approachCallsign;
        ActiveCallsign enrouteCallsign;
        ActiveCallsign enrouteCallsign2;
        ActiveCallsign deliveryCallsign;
        AirfieldOwnershipManager manager;
        ActiveCallsignCollection activeCallsigns;
        AirfieldCollection airfields;
    };

    TEST_F(AirfieldOwnershipManagerTest, AirfieldsStartWithNoDeliveryProvider)
    {
        EXPECT_FALSE(this->providers->AirfieldHasDeliveryProvider("EGGD"));
        EXPECT_TRUE(this->providers->GetServiceProviders("EGGD").empty());
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshHandlesInvalidAirfields)
    {
        EXPECT_NO_THROW(this->manager.RefreshOwner("EGFF"));
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsDeliveryProvider)
    {
        this->activeCallsigns.AddCallsign(ActiveCallsign("EGGD_TWR", "Testy McTestface", towerController, false));
        this->manager.RefreshOwner("EGGD");

        EXPECT_TRUE(this->providers->AirfieldHasDeliveryProvider("EGGD"));
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_TWR"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsDeliveryProviderWithCorrectPriority)
    {
        this->activeCallsigns.AddCallsign(towerCallsign);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_TWR"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);

        this->activeCallsigns.AddCallsign(groundCallsign);
        this->manager.RefreshOwner("EGGD");

        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_GND"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsOnlyOneDeliveryProvider)
    {
        this->activeCallsigns.AddCallsign(towerCallsign);
        this->activeCallsigns.AddCallsign(groundCallsign);
        this->activeCallsigns.AddCallsign(approachCallsign);
        this->manager.RefreshOwner("EGGD");
        const auto& providers = this->providers->GetServiceProviders("EGGD");
        auto count = std::count_if(
            providers.cbegin(), providers.cend(), [](const std::shared_ptr<ServiceProvision>& provision) -> bool {
                return provision->serviceProvided == ServiceType::Delivery;
            });
        EXPECT_EQ(1, count);
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsProvidersForDeliveryPositions)
    {
        this->activeCallsigns.AddCallsign(deliveryCallsign);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_DEL"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
        EXPECT_FALSE(this->providers->ServiceProvidedAtAirfield("EGGD", ServiceType::Ground));
        EXPECT_FALSE(this->providers->ServiceProvidedAtAirfield("EGGD", ServiceType::Tower));
        EXPECT_FALSE(this->providers->ServiceProvidedAtAirfield("EGGD", ServiceType::FinalApproach));
        EXPECT_FALSE(this->providers->ServiceProvidedAtAirfield("EGGD", ServiceType::Approach));
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsProvidersForGroundPositions)
    {
        this->activeCallsigns.AddCallsign(groundCallsign);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_GND"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
        const auto groundProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Ground);
        EXPECT_EQ(1, groundProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_GND"), *(*groundProviders.cbegin())->controller);

        EXPECT_FALSE(this->providers->ServiceProvidedAtAirfield("EGGD", ServiceType::Tower));
        EXPECT_FALSE(this->providers->ServiceProvidedAtAirfield("EGGD", ServiceType::FinalApproach));
        EXPECT_FALSE(this->providers->ServiceProvidedAtAirfield("EGGD", ServiceType::Approach));
    }

    TEST_F(AirfieldOwnershipManagerTest, MultipleGroundControllersCanProvideServices)
    {
        this->activeCallsigns.AddCallsign(groundCallsign);
        this->activeCallsigns.AddCallsign(groundCallsign2);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_GND"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
        const auto groundProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Ground);
        EXPECT_EQ(2, groundProviders.size());
        auto providerIterator = groundProviders.cbegin();
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_GND"), *(*providerIterator)->controller);
        providerIterator++;
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_2_GND"), *(*providerIterator)->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, OnlyGroundControllersProvideGroundServicesIfOnline)
    {
        this->activeCallsigns.AddCallsign(groundCallsign);
        this->activeCallsigns.AddCallsign(towerCallsign);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_GND"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
        const auto groundProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Ground);
        EXPECT_EQ(1, groundProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_GND"), *(*groundProviders.cbegin())->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsProvidersForTowerPositions)
    {
        this->activeCallsigns.AddCallsign(towerCallsign);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_TWR"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
        const auto groundProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Ground);
        EXPECT_EQ(1, groundProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_TWR"), *(*groundProviders.cbegin())->controller);

        const auto towerProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Tower);
        EXPECT_EQ(1, towerProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_TWR"), *(*towerProviders.cbegin())->controller);

        EXPECT_FALSE(this->providers->ServiceProvidedAtAirfield("EGGD", ServiceType::FinalApproach));
        EXPECT_FALSE(this->providers->ServiceProvidedAtAirfield("EGGD", ServiceType::Approach));
    }

    TEST_F(AirfieldOwnershipManagerTest, MultipleTowerControllersCanProvideServices)
    {
        this->activeCallsigns.AddCallsign(towerCallsign);
        this->activeCallsigns.AddCallsign(towerCallsign2);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_TWR"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
        const auto groundProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Ground);
        EXPECT_EQ(1, groundProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_TWR"), *groundProviders[0]->controller);

        const auto towerProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Tower);
        EXPECT_EQ(2, towerProviders.size());
        auto towerProviderIterator = towerProviders.cbegin();

        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_TWR"), *(*towerProviderIterator)->controller);
        towerProviderIterator++;
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_2_TWR"), *(*towerProviderIterator)->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, OnlyTowerControllersProvideTowerServicesIfOnline)
    {
        this->activeCallsigns.AddCallsign(approachCallsign);
        this->activeCallsigns.AddCallsign(towerCallsign);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_TWR"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
        const auto towerProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Tower);
        EXPECT_EQ(1, towerProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_TWR"), *(*towerProviders.cbegin())->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsProvidersForApproachPositions)
    {
        this->activeCallsigns.AddCallsign(approachCallsign);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_APP"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
        const auto groundProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Ground);
        EXPECT_EQ(1, groundProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_APP"), *(*groundProviders.cbegin())->controller);

        const auto towerProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Tower);
        EXPECT_EQ(1, towerProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_APP"), *(*towerProviders.cbegin())->controller);

        const auto approachProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Approach);
        EXPECT_EQ(1, approachProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_APP"), *(*approachProviders.cbegin())->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, MultipleApproachControllersCanProvideServices)
    {
        this->activeCallsigns.AddCallsign(approachCallsign);
        this->activeCallsigns.AddCallsign(finalApproachCallsign);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_F_APP"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
        const auto groundProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Ground);
        EXPECT_EQ(1, groundProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_F_APP"), *groundProviders[0]->controller);

        const auto towerProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Tower);
        EXPECT_EQ(1, towerProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_F_APP"), *towerProviders[0]->controller);

        const auto approachProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Approach);
        auto providerIterator = approachProviders.cbegin();
        EXPECT_EQ(2, approachProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_F_APP"), *(*providerIterator)->controller);
        providerIterator++;
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_APP"), *(*providerIterator)->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, OnlyApproachControllersCanProvideApproachServicesIfOnline)
    {
        this->activeCallsigns.AddCallsign(approachCallsign);
        this->activeCallsigns.AddCallsign(enrouteCallsign);
        this->manager.RefreshOwner("EGGD");
        const auto approachProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Approach);
        EXPECT_EQ(1, approachProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_APP"), *(*approachProviders.cbegin())->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, OnlyTheFirstApproachControllerProvidesFinalApproachServices)
    {
        this->activeCallsigns.AddCallsign(approachCallsign);
        this->activeCallsigns.AddCallsign(finalApproachCallsign);
        this->manager.RefreshOwner("EGGD");

        const auto finalApproachProviders =
            this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::FinalApproach);
        EXPECT_EQ(1, finalApproachProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("EGGD_F_APP"), *(*finalApproachProviders.cbegin())->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshOwnerSetsProvidersForEnroutePositions)
    {
        this->activeCallsigns.AddCallsign(enrouteCallsign);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("LON_W_CTR"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
        const auto groundProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Ground);
        EXPECT_EQ(1, groundProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("LON_W_CTR"), *(*groundProviders.cbegin())->controller);

        const auto towerProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Tower);
        EXPECT_EQ(1, towerProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("LON_W_CTR"), *(*towerProviders.cbegin())->controller);

        const auto approachProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Approach);
        EXPECT_EQ(1, approachProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("LON_W_CTR"), *(*approachProviders.cbegin())->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, OnlyOneEnroutePositionProvidesServices)
    {
        this->activeCallsigns.AddCallsign(enrouteCallsign);
        this->activeCallsigns.AddCallsign(enrouteCallsign2);
        this->manager.RefreshOwner("EGGD");

        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("LON_W_CTR"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
        const auto groundProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Ground);
        EXPECT_EQ(1, groundProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("LON_W_CTR"), *(*groundProviders.cbegin())->controller);

        const auto towerProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Tower);
        EXPECT_EQ(1, towerProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("LON_W_CTR"), *(*towerProviders.cbegin())->controller);

        const auto approachProviders = this->providers->GetProvidersForServiceAtAirfield("EGGD", ServiceType::Approach);
        EXPECT_EQ(1, approachProviders.size());
        EXPECT_EQ(this->activeCallsigns.GetCallsign("LON_W_CTR"), *(*approachProviders.cbegin())->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshRemovesAllProvidersIfNoneOnline)
    {
        this->activeCallsigns.AddCallsign(towerCallsign);
        this->manager.RefreshOwner("EGGD");
        this->activeCallsigns.RemoveCallsign(towerCallsign);
        this->manager.RefreshOwner("EGGD");

        EXPECT_TRUE(this->providers->GetServiceProviders("EGGD").empty());
    }

    TEST_F(AirfieldOwnershipManagerTest, RefreshAfterLogOffChangesDeliveryProvider)
    {
        this->activeCallsigns.AddCallsign(towerCallsign);
        this->activeCallsigns.AddCallsign(groundCallsign);

        this->manager.RefreshOwner("EGGD");

        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_GND"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);

        this->activeCallsigns.RemoveCallsign(groundCallsign);
        this->manager.RefreshOwner("EGGD");
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGGD_TWR"),
            *this->providers->DeliveryProviderForAirfield("EGGD")->controller);
    }

    TEST_F(AirfieldOwnershipManagerTest, FlushRemovesAllProviders)
    {
        this->activeCallsigns.AddCallsign(towerCallsign);
        this->activeCallsigns.AddCallsign(groundCallsign);

        this->manager.RefreshOwner("EGGD");
        this->manager.Flush();

        EXPECT_TRUE(this->providers->GetServiceProviders("EGGD").empty());
    }
} // namespace UKControllerPluginTest::Ownership
