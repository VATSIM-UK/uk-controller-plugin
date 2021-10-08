#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"

using ::testing::Test;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPlugin::Ownership::ServiceProvision;
using UKControllerPlugin::Ownership::ServiceType;

namespace UKControllerPluginTest::Ownership {

    class AirfieldServiceProviderCollectionTest : public Test
    {
        public:
        AirfieldServiceProviderCollectionTest()
            : towerController(1, "EGGD_TWR", 133.850, {"EGGD"}, true, true),
              groundController(2, "EGGD_GND", 121.920, {"EGGD"}, true, true),
              approachController(4, "EGGD_APP", 121.920, {"EGGD"}, true, true)
        {
        }

        [[nodiscard]] static auto
        CreateProvision(const ControllerPosition& position, ServiceType serviceType, bool isUser)
            -> std::shared_ptr<ServiceProvision>
        {
            return std::make_shared<ServiceProvision>(
                serviceType, std::make_shared<ActiveCallsign>(position.GetCallsign(), "Test", position, isUser));
        }

        AirfieldServiceProviderCollection collection;
        ControllerPosition towerController;
        ControllerPosition groundController;
        ControllerPosition approachController;
    };

    TEST_F(AirfieldServiceProviderCollectionTest, AirfieldsStartWithNoProviders)
    {
        EXPECT_TRUE(collection.GetServiceProviders("EGGD").empty());
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItSetsProvidersForAirfield)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Ground, true));
        providers.push_back(CreateProvision(groundController, ServiceType::Delivery, true));
        providers.push_back(CreateProvision(towerController, ServiceType::Tower, false));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_EQ(providers, collection.GetServiceProviders("EGGD"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItFlushesAllProviders)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Ground, true));
        providers.push_back(CreateProvision(towerController, ServiceType::Tower, false));
        collection.SetProvidersForAirfield("EGGD", providers);
        collection.SetProvidersForAirfield("EGFF", providers);

        collection.Flush();

        EXPECT_TRUE(collection.GetServiceProviders("EGGD").empty());
        EXPECT_TRUE(collection.GetServiceProviders("EGFF").empty());
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItFlushesProvidersForAirfield)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Ground, true));
        providers.push_back(CreateProvision(towerController, ServiceType::Tower, false));
        collection.SetProvidersForAirfield("EGGD", providers);
        collection.SetProvidersForAirfield("EGFF", providers);

        collection.FlushForAirfield("EGGD");

        EXPECT_TRUE(collection.GetServiceProviders("EGGD").empty());
        EXPECT_EQ(providers, collection.GetServiceProviders("EGFF"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsWhenUserIsntProvidingDelivery)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Delivery, false));
        providers.push_back(CreateProvision(towerController, ServiceType::Delivery, false));
        providers.push_back(CreateProvision(towerController, ServiceType::Ground, true));
        collection.SetProvidersForAirfield("EGGD", providers);
        collection.SetProvidersForAirfield("EGFF", providers);

        EXPECT_TRUE(collection.GetAirfieldsWhereUserIsProvidingDelivery().empty());
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsWhereUserIsProvidingDelivery)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Delivery, true));
        providers.push_back(CreateProvision(towerController, ServiceType::Delivery, true));
        collection.SetProvidersForAirfield("EGGD", providers);
        collection.SetProvidersForAirfield("EGFF", providers);

        std::vector<std::string> expected{"EGFF", "EGGD"};
        EXPECT_EQ(expected, collection.GetAirfieldsWhereUserIsProvidingDelivery());
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsDeliveryProviderForAirfield)
    {
        auto provider1 = CreateProvision(groundController, ServiceType::Delivery, true);

        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(provider1);
        providers.push_back(CreateProvision(towerController, ServiceType::Ground, true));
        providers.push_back(CreateProvision(towerController, ServiceType::Tower, true));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_EQ(provider1, collection.DeliveryProviderForAirfield("EGGD"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsNullptrIfNobodyProvidingDelivery)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Delivery, true));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_EQ(nullptr, collection.DeliveryProviderForAirfield("EGFF"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ReturnsIfDeliveryIsProvidedAtAirfield)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Delivery, true));
        providers.push_back(CreateProvision(towerController, ServiceType::Ground, true));
        providers.push_back(CreateProvision(towerController, ServiceType::Tower, true));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_TRUE(collection.AirfieldHasDeliveryProvider("EGGD"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ReturnsIfDeliveryIsNotProvidedAtAirfield)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Delivery, true));
        providers.push_back(CreateProvision(towerController, ServiceType::Ground, true));
        providers.push_back(CreateProvision(towerController, ServiceType::Tower, true));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_FALSE(collection.AirfieldHasDeliveryProvider("EGFF"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsDeliveryProviderForAirfieldIsUser)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Delivery, true));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_TRUE(collection.DeliveryControlProvidedByUser("EGGD"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsDeliveryProviderForAirfieldIsNotUser)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Delivery, false));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_FALSE(collection.DeliveryControlProvidedByUser("EGGD"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsFinalApproachProviderForAirfieldIsUser)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::FinalApproach, true));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_TRUE(collection.FinalApproachControlProvidedByUser("EGGD"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsFinalApproachProviderForAirfieldIsNotUser)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::FinalApproach, false));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_FALSE(collection.FinalApproachControlProvidedByUser("EGGD"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsApproachProviderForAirfieldIsUser)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Approach, true));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_TRUE(collection.ApproachControlProvidedByUser("EGGD"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsApproachProviderForAirfieldIsNotUser)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Approach, false));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_FALSE(collection.ApproachControlProvidedByUser("EGGD"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsTowerProviderForAirfieldIsUser)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Tower, true));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_TRUE(collection.TowerControlProvidedByUser("EGGD"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsTowerProviderForAirfieldIsNotUser)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Tower, false));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_FALSE(collection.TowerControlProvidedByUser("EGGD"));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsTrueIfServiceProvidedAtAirfield)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Ground, false));
        providers.push_back(CreateProvision(towerController, ServiceType::Tower, false));
        providers.push_back(CreateProvision(approachController, ServiceType::Approach, false));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_TRUE(collection.ServiceProvidedAtAirfield("EGGD", ServiceType::Ground));
        EXPECT_TRUE(collection.ServiceProvidedAtAirfield("EGGD", ServiceType::Tower));
        EXPECT_TRUE(collection.ServiceProvidedAtAirfield("EGGD", ServiceType::Approach));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsFalseNotProvidedAtAirfield)
    {
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(CreateProvision(groundController, ServiceType::Ground, false));
        providers.push_back(CreateProvision(towerController, ServiceType::Tower, false));
        providers.push_back(CreateProvision(approachController, ServiceType::Approach, false));
        collection.SetProvidersForAirfield("EGGD", providers);

        EXPECT_FALSE(collection.ServiceProvidedAtAirfield("EGGD", ServiceType::Delivery));
        EXPECT_FALSE(collection.ServiceProvidedAtAirfield("EGGD", ServiceType::FinalApproach));
    }

    TEST_F(AirfieldServiceProviderCollectionTest, ItReturnsProvidersOfServiceAtAirfield)
    {
        auto groundProvision = CreateProvision(groundController, ServiceType::Ground, false);
        auto groundProvision2 = CreateProvision(towerController, ServiceType::Ground, false);
        auto approachProvision = CreateProvision(approachController, ServiceType::Approach, false);
        std::vector<std::shared_ptr<ServiceProvision>> providers;
        providers.push_back(groundProvision);
        providers.push_back(groundProvision2);
        providers.push_back(approachProvision);
        collection.SetProvidersForAirfield("EGGD", providers);

        auto groundProviders = collection.GetProvidersForServiceAtAirfield("EGGD", ServiceType::Ground);
        EXPECT_EQ(2, groundProviders.size());
        EXPECT_EQ(groundProvision, groundProviders[0]);
        EXPECT_EQ(groundProvision2, groundProviders[1]);
    }
} // namespace UKControllerPluginTest::Ownership
