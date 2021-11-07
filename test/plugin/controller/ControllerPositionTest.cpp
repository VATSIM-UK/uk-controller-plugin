#include "controller/ControllerPosition.h"

using UKControllerPlugin::Controller::ControllerPosition;

namespace UKControllerPluginTest::Controller {

    class ControllerPositionTest : public testing::Test
    {
        public:
        ControllerPositionTest()
            : deliveryController(
                  1, "EGFF_DEL", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, false, false, true),
              groundController(
                  1, "EGFF_GND", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, false, false, true),
              towerController(
                  1, "EGFF_TWR", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, false, false, true),
              approachController(
                  1, "EGFF_APP", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, false, false, true),
              enrouteController(
                  1, "EGFF_CTR", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, false, false, true),
              fssController(1, "EGFF_FSS", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, false, false, true)
        {
        }

        ControllerPosition deliveryController;
        ControllerPosition groundController;
        ControllerPosition towerController;
        ControllerPosition approachController;
        ControllerPosition enrouteController;
        ControllerPosition fssController;
    };

    TEST_F(ControllerPositionTest, GetIdReturnsId)
    {
        EXPECT_EQ(1, approachController.GetId());
    }

    TEST_F(ControllerPositionTest, GetCallsignReturnsCallsign)
    {
        EXPECT_EQ(0, approachController.GetCallsign().compare("EGFF_APP"));
    }

    TEST_F(ControllerPositionTest, GetFrequencyReturnsFrequency)
    {
        EXPECT_EQ(125.850, approachController.GetFrequency());
    }

    TEST_F(ControllerPositionTest, GetTypeReturnsType)
    {
        EXPECT_EQ(0, approachController.GetType().compare("APP"));
    }

    TEST_F(ControllerPositionTest, GetTopdownReturnsTopdown)
    {
        std::vector<std::string> expected = {"EGGD", "EGFF"};
        EXPECT_EQ(expected, approachController.GetTopdown());
    }

    TEST_F(ControllerPositionTest, RequestsDepartureReleasesReturnsWhetherItCan)
    {
        EXPECT_TRUE(approachController.RequestsDepartureReleases());
    }

    TEST_F(ControllerPositionTest, ReceivesDepartureReleasesReturnsWhetherItCan)
    {
        EXPECT_FALSE(approachController.ReceivesDepartureReleases());
    }

    TEST_F(ControllerPositionTest, ComparisonOperatorReturnsTrueIfSame)
    {
        ControllerPosition controller2(1, "EGFF_APP", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, false);
        EXPECT_TRUE(approachController == controller2);
    }

    TEST_F(ControllerPositionTest, ComparisonOperatorReturnsTrueIfSameFrequencyDelta)
    {
        ControllerPosition controller2(1, "EGFF_APP", 125.8501, std::vector<std::string>{"EGGD", "EGFF"}, true, false);
        EXPECT_TRUE(approachController == controller2);
    }

    TEST_F(ControllerPositionTest, ComparisonOperatorReturnsFalseIfDifferentCallsign)
    {
        ControllerPosition controller2(1, "EGFF_R_APP", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, false);
        EXPECT_FALSE(approachController == controller2);
    }

    TEST_F(ControllerPositionTest, ComparisonOperatorReturnsFalseIfDifferentFrequency)
    {
        ControllerPosition controller2(1, "EGFF_APP", 125.650, std::vector<std::string>{"EGGD", "EGFF"}, true, false);
        EXPECT_FALSE(approachController == controller2);
    }

    TEST_F(ControllerPositionTest, GetUnitReturnsTheAirfieldOrAreaUnit)
    {
        EXPECT_TRUE("EGFF" == approachController.GetUnit());
    }

    TEST_F(ControllerPositionTest, ItReturnsTrueOnTopdownAirfield)
    {
        EXPECT_TRUE(approachController.HasTopdownAirfield("EGFF"));
        EXPECT_TRUE(approachController.HasTopdownAirfield("EGGD"));
    }

    TEST_F(ControllerPositionTest, ItReturnsFalseOnNoTopdownAirfield)
    {
        EXPECT_FALSE(approachController.HasTopdownAirfield("EGLL"));
    }

    TEST_F(ControllerPositionTest, SendsPrenoteMessagesReturnsWhetherItCan)
    {
        EXPECT_FALSE(approachController.SendsPrenoteMessages());
    }

    TEST_F(ControllerPositionTest, ReceivesPrenoteMessagesReturnsWhetherItCan)
    {
        EXPECT_TRUE(approachController.ReceivesPrenoteMessages());
    }

    TEST_F(ControllerPositionTest, IsDeliveryReturnsFalseNotDelivery)
    {
        EXPECT_FALSE(approachController.IsDelivery());
    }

    TEST_F(ControllerPositionTest, IsDeliveryReturnsTrueIfDelivery)
    {
        EXPECT_TRUE(deliveryController.IsDelivery());
    }

    TEST_F(ControllerPositionTest, IsGroundReturnsFalseNotGround)
    {
        EXPECT_FALSE(approachController.IsGround());
    }

    TEST_F(ControllerPositionTest, IsGroundReturnsTrueIfGround)
    {
        EXPECT_TRUE(groundController.IsGround());
    }

    TEST_F(ControllerPositionTest, IsTowerReturnsFalseNotTower)
    {
        EXPECT_FALSE(approachController.IsTower());
    }

    TEST_F(ControllerPositionTest, IsTowerReturnsTrueIfTower)
    {
        EXPECT_TRUE(towerController.IsTower());
    }

    TEST_F(ControllerPositionTest, IsApproachReturnsTrueIfApproach)
    {
        EXPECT_TRUE(approachController.IsApproach());
    }

    TEST_F(ControllerPositionTest, IsApproachReturnsFalseIfNotApproach)
    {
        EXPECT_FALSE(towerController.IsApproach());
    }

    TEST_F(ControllerPositionTest, IsEnrouteReturnsFalseNotEnroute)
    {
        EXPECT_FALSE(groundController.IsEnroute());
    }

    TEST_F(ControllerPositionTest, IsEnrouteReturnsTrueIfEnroute)
    {
        EXPECT_TRUE(enrouteController.IsEnroute());
    }

    TEST_F(ControllerPositionTest, IsFlightServiceStationReturnsFalseNotFss)
    {
        EXPECT_FALSE(groundController.IsFlightServiceStation());
    }

    TEST_F(ControllerPositionTest, IsFlightServiceStationReturnsTrueIfFss)
    {
        EXPECT_TRUE(fssController.IsFlightServiceStation());
    }

    TEST_F(ControllerPositionTest, ProvidesApproachServicesReturnsFalseForDelivery)
    {
        EXPECT_FALSE(deliveryController.ProvidesApproachServices());
    }

    TEST_F(ControllerPositionTest, ProvidesApproachServicesReturnsFalseForGround)
    {
        EXPECT_FALSE(groundController.ProvidesApproachServices());
    }

    TEST_F(ControllerPositionTest, ProvidesApproachServicesReturnsFalseForTower)
    {
        EXPECT_FALSE(towerController.ProvidesApproachServices());
    }

    TEST_F(ControllerPositionTest, ProvidesApproachServicesReturnsTrueForApproach)
    {
        EXPECT_TRUE(approachController.ProvidesApproachServices());
    }

    TEST_F(ControllerPositionTest, ProvidesApproachServicesReturnsTrueForEnroute)
    {
        EXPECT_TRUE(enrouteController.ProvidesApproachServices());
    }

    TEST_F(ControllerPositionTest, ProvidesApproachServicesReturnsFalseForFlightServiceStation)
    {
        EXPECT_FALSE(fssController.ProvidesApproachServices());
    }

    TEST_F(ControllerPositionTest, ProvidesGroundServicesReturnsFalseForDelivery)
    {
        EXPECT_FALSE(deliveryController.ProvidesGroundServices());
    }

    TEST_F(ControllerPositionTest, ProvidesGroundServicesReturnsFalseForFss)
    {
        EXPECT_FALSE(fssController.ProvidesGroundServices());
    }

    TEST_F(ControllerPositionTest, ProvidesGroundServicesReturnsTrueForGround)
    {
        EXPECT_TRUE(groundController.ProvidesGroundServices());
    }

    TEST_F(ControllerPositionTest, ProvidesGroundServicesReturnsTrueForTower)
    {
        EXPECT_TRUE(towerController.ProvidesGroundServices());
    }

    TEST_F(ControllerPositionTest, ProvidesGroundServicesReturnsTrueForApproach)
    {
        EXPECT_TRUE(approachController.ProvidesGroundServices());
    }

    TEST_F(ControllerPositionTest, ProvidesGroundServicesReturnsTrueForEnroute)
    {
        EXPECT_TRUE(enrouteController.ProvidesGroundServices());
    }

    TEST_F(ControllerPositionTest, ProvidesTowerServicesReturnsFalseForDelivery)
    {
        EXPECT_FALSE(deliveryController.ProvidesTowerServices());
    }

    TEST_F(ControllerPositionTest, ProvidesTowerServicesReturnsFalseForFss)
    {
        EXPECT_FALSE(fssController.ProvidesTowerServices());
    }

    TEST_F(ControllerPositionTest, ProvidesTowerServicesReturnsFalseForGround)
    {
        EXPECT_FALSE(groundController.ProvidesTowerServices());
    }

    TEST_F(ControllerPositionTest, ProvidesTowerServicesReturnsTrueForTower)
    {
        EXPECT_TRUE(towerController.ProvidesTowerServices());
    }

    TEST_F(ControllerPositionTest, ProvidesTowerServicesReturnsTrueForApproach)
    {
        EXPECT_TRUE(approachController.ProvidesTowerServices());
    }

    TEST_F(ControllerPositionTest, ProvidesTowerServicesReturnsTrueForEnroute)
    {
        EXPECT_TRUE(enrouteController.ProvidesTowerServices());
    }
} // namespace UKControllerPluginTest::Controller
