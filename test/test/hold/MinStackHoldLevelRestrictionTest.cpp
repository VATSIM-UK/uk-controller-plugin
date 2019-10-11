#include "pch/pch.h"
#include "hold/MinStackHoldLevelRestriction.h"
#include "minstack/MinStackManager.h"
#include "sectorfile/RunwayCollection.h"
#include "mock/MockSectorFileProviderInterface.h"
#include "mock/MockEuroscopeSectorFileElementInterface.h"

using UKControllerPlugin::Hold::MinStackHoldLevelRestriction;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::SectorFile::RunwayCollection;
using UKControllerPluginTest::SectorFile::MockSectorFileProviderInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopeSectorFileElementInterface;
using UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Hold {

        class MinStackHoldLevelRestrictionTest : public Test
        {
            public:
                MinStackHoldLevelRestrictionTest()
                    : runways(sectorFileProvider), restriction("EGLL", 2000, 8000, manager, runways, "")
                {
                    manager.AddMsl("airfield.EGLL", "airfield", "Heathrow", 7000);


                    // Setup the first element
                    element1.reset(new NiceMock<MockEuroscopeSectorFileElementInterface>);
                    ON_CALL(*element1, Airport())
                        .WillByDefault(Return("EGLL - London Heathrow"));

                    ON_CALL(*element1, Name())
                        .WillByDefault(Return("EGLL - London Heathrow 09R - 27L"));

                    ON_CALL(*element1, Runway1ActiveForDepartures())
                        .WillByDefault(Return(false));

                    ON_CALL(*element1, Runway1ActiveForArrivals())
                        .WillByDefault(Return(false));

                    ON_CALL(*element1, Runway2ActiveForDepartures())
                        .WillByDefault(Return(true));

                    ON_CALL(*element1, Runway2ActiveForArrivals())
                        .WillByDefault(Return(true));

                    ON_CALL(*element1, Runway1Heading())
                        .WillByDefault(Return(89));

                    ON_CALL(*element1, Runway2Heading())
                        .WillByDefault(Return(271));

                    ON_CALL(*element1, Runway1Identifier())
                        .WillByDefault(Return("09R"));

                    ON_CALL(*element1, Runway2Identifier())
                        .WillByDefault(Return("27L"));

                    std::set<std::shared_ptr<EuroscopeSectorFileElementInterface>> elements = {element1};

                    ON_CALL(sectorFileProvider, GetAllElementsByType(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY))
                        .WillByDefault(Return(elements));
                }

                std::shared_ptr<NiceMock<MockEuroscopeSectorFileElementInterface>> element1;
                NiceMock<MockSectorFileProviderInterface> sectorFileProvider;
                RunwayCollection runways;
                MinStackManager manager;
                MinStackHoldLevelRestriction restriction;
        };

        TEST_F(MinStackHoldLevelRestrictionTest, IsLevelRestrictedReturnsTrueIfLevelBelowMinimum)
        {
            EXPECT_TRUE(this->restriction.LevelRestricted(8000));
        }

        TEST_F(MinStackHoldLevelRestrictionTest, IsLevelRestrictedReturnsFalseIfLevelAtMinimum)
        {
            EXPECT_FALSE(this->restriction.LevelRestricted(9000));
        }

        TEST_F(MinStackHoldLevelRestrictionTest, IsLevelRestrictedReturnsFalseIfLevelAboveMinimum)
        {
            EXPECT_FALSE(this->restriction.LevelRestricted(10000));
        }

        TEST_F(MinStackHoldLevelRestrictionTest, IsLevelRestrictedReturnsTrueIfBelowOverride)
        {
            MinStackHoldLevelRestriction restriction2("EGLL", 1000, 9000, manager, runways, "");
            EXPECT_TRUE(restriction2.LevelRestricted(8000));
        }

        TEST_F(MinStackHoldLevelRestrictionTest, IsLevelRestrictedReturnsFalseNoMsl)
        {
            MinStackHoldLevelRestriction restriction2("EGLL", 1000, 7000, manager, runways, "");
            EXPECT_FALSE(restriction2.LevelRestricted(8000));
        }

        TEST_F(MinStackHoldLevelRestrictionTest, IsLevelRestrictedReturnsFalseRunwayNotFound)
        {
            MinStackHoldLevelRestriction restriction2("EGLL", 1000, 7000, manager, runways, "27R");
            EXPECT_FALSE(restriction2.LevelRestricted(8000));
        }

        TEST_F(MinStackHoldLevelRestrictionTest, IsLevelRestrictedReturnsTrueIfRestrictedAndRunwayActive)
        {
            MinStackHoldLevelRestriction restriction2("EGLL", 1000, 7000, manager, runways, "27L");
            EXPECT_FALSE(restriction2.LevelRestricted(7000));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
