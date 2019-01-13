#include "pch/pch.h"
#include "intention/SectorExitPoint.h"

using UKControllerPlugin::IntentionCode::SectorExitPoint;

namespace UKControllerPluginTest {
    namespace IntentionCode {

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseNegativeNumber)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorth);
            EXPECT_FALSE(exit.IsCorrectOutDirection(-10.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseTooLarge)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorth);
            EXPECT_FALSE(exit.IsCorrectOutDirection(380));
        }

        // North
        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueNorth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorth);
            EXPECT_TRUE(exit.IsCorrectOutDirection(360.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueNorthBoundaryWest)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorth);
            EXPECT_TRUE(exit.IsCorrectOutDirection(305.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseNorthTooFarWest)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorth);
            EXPECT_FALSE(exit.IsCorrectOutDirection(255.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueNorthBoundaryEast)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorth);
            EXPECT_TRUE(exit.IsCorrectOutDirection(55.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseNorthTooFarEast)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorth);
            EXPECT_FALSE(exit.IsCorrectOutDirection(65.0));
        }

        // East
        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueEast)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outEast);
            EXPECT_TRUE(exit.IsCorrectOutDirection(90.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueEastBoundaryNorth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outEast);
            EXPECT_TRUE(exit.IsCorrectOutDirection(35.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseEastTooFarNorth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outEast);
            EXPECT_FALSE(exit.IsCorrectOutDirection(25.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueEastBoundarySouth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outEast);
            EXPECT_TRUE(exit.IsCorrectOutDirection(145.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseEastTooFarSouth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outEast);
            EXPECT_FALSE(exit.IsCorrectOutDirection(150.0));
        }


        // South
        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueSouth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouth);
            EXPECT_TRUE(exit.IsCorrectOutDirection(180.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueSouthBoundaryEast)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouth);
            EXPECT_TRUE(exit.IsCorrectOutDirection(125.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseEastTooFarEast)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouth);
            EXPECT_FALSE(exit.IsCorrectOutDirection(115.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueSouthBoundaryWest)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouth);
            EXPECT_TRUE(exit.IsCorrectOutDirection(235.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseSouthTooFarWest)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouth);
            EXPECT_FALSE(exit.IsCorrectOutDirection(250.0));
        }

        // West

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueWest)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outWest);
            EXPECT_TRUE(exit.IsCorrectOutDirection(270.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueWestBoundarySouth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outWest);
            EXPECT_TRUE(exit.IsCorrectOutDirection(215.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseWestTooFarSouth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outWest);
            EXPECT_FALSE(exit.IsCorrectOutDirection(210.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueWestBoundaryNorth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outWest);
            EXPECT_TRUE(exit.IsCorrectOutDirection(325.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseWestTooFarNorth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outWest);
            EXPECT_FALSE(exit.IsCorrectOutDirection(335.0));
        }

        // South East

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueSouthEast)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouthEast);
            EXPECT_TRUE(exit.IsCorrectOutDirection(135.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueSouthEastBoundaryNorth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouthEast);
            EXPECT_TRUE(exit.IsCorrectOutDirection(55.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseSouthEastTooFarNorth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouthEast);
            EXPECT_FALSE(exit.IsCorrectOutDirection(45.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueSouthEastBoundarySouth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouthEast);
            EXPECT_TRUE(exit.IsCorrectOutDirection(180.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseSouthEastTooFarSouth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouthEast);
            EXPECT_FALSE(exit.IsCorrectOutDirection(185.0));
        }

        // South West

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueSouthWest)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouthWest);
            EXPECT_TRUE(exit.IsCorrectOutDirection(225.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueSouthWestBoundaryNorth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouthWest);
            EXPECT_TRUE(exit.IsCorrectOutDirection(305.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseSouthWestTooFarNorth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouthWest);
            EXPECT_FALSE(exit.IsCorrectOutDirection(310.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueSouthWestBoundarySouth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouthWest);
            EXPECT_TRUE(exit.IsCorrectOutDirection(180.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseSouthWestTooFarSouth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outSouthWest);
            EXPECT_FALSE(exit.IsCorrectOutDirection(175.0));
        }

        // North West

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueNorthWest)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorthWest);
            EXPECT_TRUE(exit.IsCorrectOutDirection(315.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueNorthWestBoundaryNorth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorthWest);
            EXPECT_TRUE(exit.IsCorrectOutDirection(360.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseNorthWestTooFarNorth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorthWest);
            EXPECT_FALSE(exit.IsCorrectOutDirection(5.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueNorthWestBoundarySouth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorthWest);
            EXPECT_TRUE(exit.IsCorrectOutDirection(260.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseNorthWestTooFarSouth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorthWest);
            EXPECT_FALSE(exit.IsCorrectOutDirection(259.0));
        }

        // North East

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueNorthEast)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorthEast);
            EXPECT_TRUE(exit.IsCorrectOutDirection(90.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueNorthEastBoundaryNorth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorthEast);
            EXPECT_TRUE(exit.IsCorrectOutDirection(0.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseNorthEastTooFarNorth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorthEast);
            EXPECT_FALSE(exit.IsCorrectOutDirection(355.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsTrueNorthEastBoundarySouth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorthEast);
            EXPECT_TRUE(exit.IsCorrectOutDirection(125.0));
        }

        TEST(SectorExitPoint, IsCorrectOutDirectionReturnsFalseNorthEastTooFarSouth)
        {
            SectorExitPoint exit("Test", "T", SectorExitPoint::outNorthEast);
            EXPECT_FALSE(exit.IsCorrectOutDirection(126.0));
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
