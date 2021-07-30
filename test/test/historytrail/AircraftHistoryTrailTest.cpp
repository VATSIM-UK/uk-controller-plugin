#include "pch/pch.h"
#include "historytrail/AircraftHistoryTrail.h"

using UKControllerPlugin::HistoryTrail::AircraftHistoryTrail;

namespace UKControllerPluginTest {
    namespace HistoryTrail {

        TEST(HistoryTrail, AddItemAddsItemQueueEmpty)
        {
            AircraftHistoryTrail history("test");

            // Create a fake position
            EuroScopePlugIn::CPosition positionTest;
            positionTest.m_Latitude = 1;
            positionTest.m_Longitude = 2;

            // Add to the history trail, then get the trail to check values.
            history.AddItem({123, positionTest});
            auto trail = history.GetTrail();

            // Check the trail.
            EXPECT_EQ(1, trail.size());
            EXPECT_EQ(1, trail.front().position.m_Latitude);
            EXPECT_EQ(2, trail.front().position.m_Longitude);
            EXPECT_EQ(123, trail.front().heading);
        }

        TEST(HistoryTrail, AddItemAddsItemIfQueueNotFull)
        {
            AircraftHistoryTrail history("test");

            // Create a fake position
            EuroScopePlugIn::CPosition positionTest;
            positionTest.m_Latitude = 1;
            positionTest.m_Longitude = 2;

            // Add to the history trail, then get the trail to check values.
            history.AddItem({123, positionTest});
            auto trail = history.GetTrail();

            // Check the trail.
            EXPECT_EQ(1, trail.size());
            EXPECT_EQ(1, trail.front().position.m_Latitude);
            EXPECT_EQ(2, trail.front().position.m_Longitude);
            EXPECT_EQ(123, trail.front().heading);
        }

        TEST(HistoryTrail, AddItemAddsItemIfQueueNotFullBoundary)
        {
            AircraftHistoryTrail history("test");

            // Create a fake position
            EuroScopePlugIn::CPosition positionTest;
            positionTest.m_Latitude = 1;
            positionTest.m_Longitude = 2;

            // Add to the history trail, then get the trail to check values.
            history.AddItem({123, positionTest});
            auto trail = history.GetTrail();

            // Check the trail.
            EXPECT_EQ(1, trail.size());
            EXPECT_EQ(1, trail.front().position.m_Latitude);
            EXPECT_EQ(2, trail.front().position.m_Longitude);
            EXPECT_EQ(123, trail.front().heading);
        }


        TEST(HistoryTrail, AddItemTrailItemsMaintainOrder)
        {
            AircraftHistoryTrail history("test");

            // Create a fake position
            EuroScopePlugIn::CPosition positionTestFirst;
            positionTestFirst.m_Latitude = 1;
            positionTestFirst.m_Longitude = 2;

            EuroScopePlugIn::CPosition positionTestSecond;
            positionTestSecond.m_Latitude = 3;
            positionTestSecond.m_Longitude = 4;

            // Add to the history trail, then get the trail to check values.
            history.AddItem({123, positionTestFirst});
            history.AddItem({456, positionTestSecond});
            auto trail = history.GetTrail();

            // Check the trail.
            EXPECT_EQ(2, trail.size());

            // First item
            EXPECT_EQ(1, trail.back().position.m_Latitude);
            EXPECT_EQ(2, trail.back().position.m_Longitude);
            EXPECT_EQ(123, trail.back().heading);

            // Second item
            trail.pop_back();
            EXPECT_EQ(3, trail.back().position.m_Latitude);
            EXPECT_EQ(4, trail.back().position.m_Longitude);
            EXPECT_EQ(456, trail.back().heading);
        }

        TEST(HistoryTrail, AddItemTrailRemovesLastItemIfFull)
        {
            AircraftHistoryTrail history("test");

            // Create a fake position
            EuroScopePlugIn::CPosition positionTestFirst;
            positionTestFirst.m_Latitude = 1;
            positionTestFirst.m_Longitude = 2;

            EuroScopePlugIn::CPosition positionTestSecond;
            positionTestSecond.m_Latitude = 3;
            positionTestSecond.m_Longitude = 4;

            // Fill up the trail with the first value
            for (unsigned int i = 0; i < history.maxSize; i++) {
                history.AddItem({123, positionTestFirst});
            }

            // Add a second value to test with
            history.AddItem({456, positionTestSecond});
            auto trail = history.GetTrail();

            // Check the trail.
            EXPECT_EQ(history.maxSize, trail.size());

            // First item
            EXPECT_EQ(3, trail.front().position.m_Latitude);
            EXPECT_EQ(4, trail.front().position.m_Longitude);
            EXPECT_EQ(456, trail.front().heading);
        }
    }  // namespace HistoryTrail
}  // namespace UKControllerPluginTest
