#include "approach/GlideslopeDeviationEstimator.h"
#include "approach/GlideslopeDeviationTagItem.h"
#include "runway/Runway.h"
#include "runway/RunwayCollection.h"
#include "tag/TagData.h"

namespace UKControllerPluginTest::Approach {
    class GlideslopeDeviationTagItemTest : public testing::Test
    {
        public:
        GlideslopeDeviationTagItemTest()
            : tagData(
                  mockFlightplan,
                  mockRadarTarget,
                  132,
                  EuroScopePlugIn::TAG_DATA_CORRELATED,
                  itemString,
                  &euroscopeColourCode,
                  &tagColour,
                  &fontSize),
              runways(std::make_shared<UKControllerPlugin::Runway::RunwayCollection>()),
              glideslopeDeviationEstimator(
                  std::make_shared<UKControllerPlugin::Approach::GlideslopeDeviationEstimator>()),
              glideslopeDeviationTagItem(glideslopeDeviationEstimator, runways)
        {
            // Add a runway to the collection, based on Gatwick's 26L
            EuroScopePlugIn::CPosition pos;
            pos.m_Latitude = 51.150675;
            pos.m_Longitude = -0.171925;
            runways->Add(std::make_shared<UKControllerPlugin::Runway::Runway>(1, 1, "26L", 257, pos));

            // Flightplan
            ON_CALL(mockFlightplan, GetCallsign()).WillByDefault(testing::Return("BAW123"));
            ON_CALL(mockFlightplan, GetDestination()).WillByDefault(testing::Return("EGKK"));
            ON_CALL(mockFlightplan, GetArrivalRunway()).WillByDefault(testing::Return("26L"));

            // Aircraft position
            EuroScopePlugIn::CPosition aircraftPosition;
            aircraftPosition.m_Latitude = 51.17575;
            aircraftPosition.m_Longitude = 0.00829;
            ON_CALL(mockRadarTarget, GetPosition()).WillByDefault(testing::Return(aircraftPosition));
        }

        double fontSize = 24.1;
        COLORREF tagColour = RGB(255, 255, 255);
        int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
        char itemString[16] = "Foooooo";
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> mockRadarTarget;
        UKControllerPlugin::Tag::TagData tagData;

        std::shared_ptr<UKControllerPlugin::Runway::RunwayCollection> runways;
        std::shared_ptr<UKControllerPlugin::Approach::GlideslopeDeviationEstimator> glideslopeDeviationEstimator;
        UKControllerPlugin::Approach::GlideslopeDeviationTagItem glideslopeDeviationTagItem;
    };

    TEST_F(GlideslopeDeviationTagItemTest, ItHasATagItemDescription)
    {
        EXPECT_EQ("Glideslope Deviation", glideslopeDeviationTagItem.GetTagItemDescription(132));
    }

    TEST_F(GlideslopeDeviationTagItemTest, ItThrowsExceptionIfAskedAboutInvalidTagItem)
    {
        EXPECT_THROW(glideslopeDeviationTagItem.GetTagItemDescription(0), std::invalid_argument);
    }

    TEST_F(GlideslopeDeviationTagItemTest, ItSetsTagItemDataWellAboveGlideslope)
    {
        ON_CALL(mockRadarTarget, GetAltitude()).WillByDefault(testing::Return(2896));

        glideslopeDeviationTagItem.SetTagItemData(tagData);

        EXPECT_EQ("+448", tagData.GetItemString());
        EXPECT_EQ(RGB(255, 87, 51), tagData.GetTagColour());
    }

    TEST_F(GlideslopeDeviationTagItemTest, ItSetsTagItemDataSlightlyAbove)
    {
        ON_CALL(mockRadarTarget, GetAltitude()).WillByDefault(testing::Return(2496));

        glideslopeDeviationTagItem.SetTagItemData(tagData);

        EXPECT_EQ("+48", tagData.GetItemString());
        EXPECT_EQ(RGB(2, 48, 32), tagData.GetTagColour());
    }

    TEST_F(GlideslopeDeviationTagItemTest, ItSetsTagItemDataWellBelowGlideslope)
    {
        ON_CALL(mockRadarTarget, GetAltitude()).WillByDefault(testing::Return(2000));

        glideslopeDeviationTagItem.SetTagItemData(tagData);

        EXPECT_EQ("-448", tagData.GetItemString());
        EXPECT_EQ(RGB(255, 87, 51), tagData.GetTagColour());
    }

    TEST_F(GlideslopeDeviationTagItemTest, ItSetsTagItemDataSlightlyBelow)
    {
        ON_CALL(mockRadarTarget, GetAltitude()).WillByDefault(testing::Return(2400));

        glideslopeDeviationTagItem.SetTagItemData(tagData);

        EXPECT_EQ("-48", tagData.GetItemString());
        EXPECT_EQ(RGB(2, 48, 32), tagData.GetTagColour());
    }

    TEST_F(GlideslopeDeviationTagItemTest, ItSetsTagItemDataOver1000FeetAboveGlideslope)
    {
        ON_CALL(mockRadarTarget, GetAltitude()).WillByDefault(testing::Return(5000));

        glideslopeDeviationTagItem.SetTagItemData(tagData);

        EXPECT_EQ(">1k", tagData.GetItemString());
        EXPECT_EQ(RGB(255, 87, 51), tagData.GetTagColour());
    }

    TEST_F(GlideslopeDeviationTagItemTest, ItSetsTagItemDataOver1000FeetBelowGlideslope)
    {
        ON_CALL(mockRadarTarget, GetAltitude()).WillByDefault(testing::Return(500));

        glideslopeDeviationTagItem.SetTagItemData(tagData);

        EXPECT_EQ("<1k", tagData.GetItemString());
        EXPECT_EQ(RGB(255, 87, 51), tagData.GetTagColour());
    }

    TEST_F(GlideslopeDeviationTagItemTest, ItDoesntSetTagItemDataIfRunwayNotFound)
    {
        ON_CALL(mockFlightplan, GetDestination()).WillByDefault(testing::Return("EGSS"));
        ON_CALL(mockRadarTarget, GetAltitude()).WillByDefault(testing::Return(2000));

        glideslopeDeviationTagItem.SetTagItemData(tagData);

        EXPECT_EQ("Foooooo", tagData.GetItemString());
    }

    TEST_F(GlideslopeDeviationTagItemTest, ItDoesntSetTagItemDataIfUpwindOfRunway)
    {
        EuroScopePlugIn::CPosition aircraftPosition;
        aircraftPosition.m_Latitude = 51.06744;
        aircraftPosition.m_Longitude = -0.1977;
        ON_CALL(mockRadarTarget, GetPosition()).WillByDefault(testing::Return(aircraftPosition));
        ON_CALL(mockRadarTarget, GetAltitude()).WillByDefault(testing::Return(2000));

        glideslopeDeviationTagItem.SetTagItemData(tagData);

        EXPECT_EQ("Foooooo", tagData.GetItemString());
    }

    TEST_F(GlideslopeDeviationTagItemTest, ItDoesntSetTagItemDataIfMoreThan15MilesFromLocaliser)
    {
        EuroScopePlugIn::CPosition aircraftPosition;
        aircraftPosition.m_Latitude = 51.17575;
        aircraftPosition.m_Longitude = 2.00829;
        ON_CALL(mockRadarTarget, GetPosition()).WillByDefault(testing::Return(aircraftPosition));
        ON_CALL(mockRadarTarget, GetAltitude()).WillByDefault(testing::Return(2000));

        glideslopeDeviationTagItem.SetTagItemData(tagData);

        EXPECT_EQ("Foooooo", tagData.GetItemString());
    }

    TEST_F(GlideslopeDeviationTagItemTest, ItDoesntSetTagItemDataIfMoreThan25MilesOut)
    {
        EuroScopePlugIn::CPosition aircraftPosition;
        aircraftPosition.m_Latitude = 54.22613;
        aircraftPosition.m_Longitude = 0.00829;
        ON_CALL(mockRadarTarget, GetPosition()).WillByDefault(testing::Return(aircraftPosition));
        ON_CALL(mockRadarTarget, GetAltitude()).WillByDefault(testing::Return(2000));

        glideslopeDeviationTagItem.SetTagItemData(tagData);

        EXPECT_EQ("Foooooo", tagData.GetItemString());
    }
} // namespace UKControllerPluginTest::Approach
