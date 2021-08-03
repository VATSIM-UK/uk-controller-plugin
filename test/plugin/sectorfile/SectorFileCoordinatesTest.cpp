#pragma once
#include "pch/pch.h"
#include "sectorfile/SectorFileCoordinates.h"

using UKControllerPlugin::SectorFile::GetInvalidPosition;
using UKControllerPlugin::SectorFile::PositionIsInvalid;
using UKControllerPlugin::SectorFile::ParseSectorFileCoordinates;
using testing::Test;

namespace UKControllerPluginTest {
    namespace SectorFile {

        class SectorFileCoordinatesTest : public Test
        {
            public:
        };

        TEST_F(SectorFileCoordinatesTest, ItHasAnInvalidPosition)
        {
            EXPECT_EQ(-9999.0, GetInvalidPosition().m_Latitude);
            EXPECT_EQ(-9999.0, GetInvalidPosition().m_Longitude);
        }

        TEST_F(SectorFileCoordinatesTest, InvalidPositionReturnsTrueIfInvalid)
        {
            EXPECT_TRUE(PositionIsInvalid(GetInvalidPosition()));
        }

        TEST_F(SectorFileCoordinatesTest, InvalidPositionReturnsFalseIfValid)
        {
            EuroScopePlugIn::CPosition position = GetInvalidPosition();
            position.m_Longitude = 55;
            EXPECT_FALSE(PositionIsInvalid(position));
        }

        TEST_F(SectorFileCoordinatesTest, ItParsesNorthLatitudeCoordinates)
        {
            EXPECT_NEAR(50.94556, ParseSectorFileCoordinates("N050.56.44.000", "E000.15.42.000").m_Latitude, 0.00001);
        }

        TEST_F(SectorFileCoordinatesTest, ItParsesSouthLatitudeCoordinates)
        {
            EXPECT_NEAR(-50.94556, ParseSectorFileCoordinates("S050.56.44.000", "E000.15.42.000").m_Latitude, 0.00001);
        }

        TEST_F(SectorFileCoordinatesTest, ItParsesEastLongitudeCoordinates)
        {
            EXPECT_NEAR(0.26167, ParseSectorFileCoordinates("N050.56.44.000", "E000.15.42.000").m_Longitude, 0.00001);
        }

        TEST_F(SectorFileCoordinatesTest, ItParsesWestLongitudeCoordinates)
        {
            EXPECT_NEAR(-0.26167, ParseSectorFileCoordinates("N050.56.44.000", "W000.15.42.000").m_Longitude, 0.00001);
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsInvalidPositionIfLatitudeDoesntMatchPattern)
        {
            EXPECT_TRUE(PositionIsInvalid(ParseSectorFileCoordinates("N050.56.4.000", "W000.15.42.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsInvalidPositionIfLongitudeDoesntMatchPattern)
        {
            EXPECT_TRUE(PositionIsInvalid(ParseSectorFileCoordinates("N050.56.44.000", "W000.15.2.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsInvalidPositionIfDegreesLatitudeGreaterThan90)
        {
            EXPECT_TRUE(PositionIsInvalid(ParseSectorFileCoordinates("N091.00.00.000", "W000.15.42.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsInvalidPositionIfDegreesLatitude90WithMinutes)
        {
            EXPECT_TRUE(PositionIsInvalid(ParseSectorFileCoordinates("N090.56.00.000", "W000.15.42.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsInvalidPositionIfDegreesLatitude90WithSeconds)
        {
            EXPECT_TRUE(PositionIsInvalid(ParseSectorFileCoordinates("N090.00.01.000", "W000.15.42.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsInvalidPositionIfMinutesLatitudeGreaterThan59)
        {
            EXPECT_TRUE(PositionIsInvalid(ParseSectorFileCoordinates("N050.60.44.000", "W000.15.42.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsValidPositionIfMinutesLatitudeEqualTo59)
        {
            EXPECT_FALSE(PositionIsInvalid(ParseSectorFileCoordinates("N050.59.44.000", "W000.15.42.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsInvalidPositionIfSecondsLatitudeGreaterThanOrEqualTo60)
        {
            EXPECT_TRUE(PositionIsInvalid(ParseSectorFileCoordinates("N050.59.60.000", "W000.15.42.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsValidPositionIfMinutesSecondsLessThan60)
        {
            EXPECT_FALSE(PositionIsInvalid(ParseSectorFileCoordinates("N050.59.59.888", "W000.15.42.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsInvalidPositionIfDegreesLongitudeGreaterThan180)
        {
            EXPECT_TRUE(PositionIsInvalid(ParseSectorFileCoordinates("N050.42.32.000", "W181.00.00.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsInvalidPositionIfDegreesLongitude180WithMinutes)
        {
            EXPECT_TRUE(PositionIsInvalid(ParseSectorFileCoordinates("N050.42.32.000", "W180.15.00.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsInvalidPositionIfDegreesLongitude180WithSeconds)
        {
            EXPECT_TRUE(PositionIsInvalid(ParseSectorFileCoordinates("N050.42.32.000", "W180.00.42.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsInvalidPositionIfMinutesLongitudeGreaterThan59)
        {
            EXPECT_TRUE(PositionIsInvalid(ParseSectorFileCoordinates("N050.42.32.000", "W001.60.42.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsValidPositionIfLongitudeMinutesLongitudeEqualTo59)
        {
            EXPECT_FALSE(PositionIsInvalid(ParseSectorFileCoordinates("N050.42.32.000", "W001.59.42.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsInvalidPositionIfSecondsLongitudeGreaterThanOrEqualTo60)
        {
            EXPECT_TRUE(PositionIsInvalid(ParseSectorFileCoordinates("N050.42.32.000", "W001.15.60.000")));
        }

        TEST_F(SectorFileCoordinatesTest, ItReturnsValidPositionIfSecondsLongitudeLessThan60)
        {
            EXPECT_FALSE(PositionIsInvalid(ParseSectorFileCoordinates("N050.42.32.000", "W001.15.59.888")));
        }
    }  // namespace SectorFile
}  // namespace UKControllerPluginTest
