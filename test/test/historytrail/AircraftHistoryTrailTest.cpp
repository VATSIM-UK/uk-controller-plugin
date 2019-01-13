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
            history.AddItem(positionTest);
            std::deque<EuroScopePlugIn::CPosition> trail = history.GetTrail();

            // Check the trail.
            EXPECT_EQ(1, trail.size());
            EXPECT_EQ(1, trail.front().m_Latitude);
            EXPECT_EQ(2, trail.front().m_Longitude);
        }

        TEST(HistoryTrail, AddItemAddsItemIfQueueNotFull)
        {
            AircraftHistoryTrail history("test");

            // Create a fake position
            EuroScopePlugIn::CPosition positionTest;
            positionTest.m_Latitude = 1;
            positionTest.m_Longitude = 2;

            // Add to the history trail, then get the trail to check values.
            history.AddItem(positionTest);
            std::deque<EuroScopePlugIn::CPosition> trail = history.GetTrail();

            // Check the trail.
            EXPECT_EQ(1, trail.size());
            EXPECT_EQ(1, trail.front().m_Latitude);
            EXPECT_EQ(2, trail.front().m_Longitude);
        }

        TEST(HistoryTrail, AddItemAddsItemIfQueueNotFullBoundary)
        {
            AircraftHistoryTrail history("test");

            // Create a fake position
            EuroScopePlugIn::CPosition positionTest;
            positionTest.m_Latitude = 1;
            positionTest.m_Longitude = 2;

            // Add to the history trail, then get the trail to check values.
            history.AddItem(positionTest);
            std::deque<EuroScopePlugIn::CPosition> trail = history.GetTrail();

            // Check the trail.
            EXPECT_EQ(1, trail.size());
            EXPECT_EQ(1, trail.front().m_Latitude);
            EXPECT_EQ(2, trail.front().m_Longitude);
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
            history.AddItem(positionTestFirst);
            history.AddItem(positionTestSecond);
            std::deque<EuroScopePlugIn::CPosition> trail = history.GetTrail();

            // Check the trail.
            EXPECT_EQ(2, trail.size());

            // First item
            EXPECT_EQ(1, trail.back().m_Latitude);
            EXPECT_EQ(2, trail.back().m_Longitude);

            // Second item
            trail.pop_back();
            EXPECT_EQ(3, trail.back().m_Latitude);
            EXPECT_EQ(4, trail.back().m_Longitude);
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
                history.AddItem(positionTestFirst);
            }

            // Add a second value to test with
            history.AddItem(positionTestSecond);
            std::deque<EuroScopePlugIn::CPosition> trail = history.GetTrail();

            // Check the trail.
            EXPECT_EQ(history.maxSize, trail.size());

            // First item
            EXPECT_EQ(3, trail.front().m_Latitude);
            EXPECT_EQ(4, trail.front().m_Longitude);
        }
    }  // namespace HistoryTrail
}  // namespace UKControllerPluginTest
