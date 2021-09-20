#include "sectorfile/Runway.h"
#include "sectorfile/RunwayCollection.h"
#include "euroscope/UserSetting.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::SectorFile::Runway;
using UKControllerPlugin::SectorFile::RunwayCollection;
using UKControllerPluginTest::Euroscope::MockEuroscopeSectorFileElementInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::SectorFile::MockSectorFileProviderInterface;

namespace UKControllerPluginTest {
    namespace SectorFile {

        class RunwayCollectionTest : public Test
        {
            public:
            RunwayCollectionTest() : userSetting(mockUserSettings), collection(mockSectorFile)
            {
                element1.reset(new NiceMock<MockEuroscopeSectorFileElementInterface>);
                element2.reset(new NiceMock<MockEuroscopeSectorFileElementInterface>);
                element3.reset(new NiceMock<MockEuroscopeSectorFileElementInterface>);
                element4.reset(new NiceMock<MockEuroscopeSectorFileElementInterface>);
            }

            void SetUpDefaultElements(void)
            {
                // Setup the first element
                ON_CALL(*element1, Airport()).WillByDefault(Return("EGKK - London Gatwick"));

                ON_CALL(*element1, Name()).WillByDefault(Return("EGKK London Gatwick 08R - 26L"));

                ON_CALL(*element1, Runway1ActiveForDepartures()).WillByDefault(Return(false));

                ON_CALL(*element1, Runway1ActiveForArrivals()).WillByDefault(Return(false));

                ON_CALL(*element1, Runway2ActiveForDepartures()).WillByDefault(Return(true));

                ON_CALL(*element1, Runway2ActiveForArrivals()).WillByDefault(Return(true));

                ON_CALL(*element1, Runway1Heading()).WillByDefault(Return(81));

                ON_CALL(*element1, Runway2Heading()).WillByDefault(Return(259));

                ON_CALL(*element1, Runway1Identifier()).WillByDefault(Return("08R"));

                ON_CALL(*element1, Runway2Identifier()).WillByDefault(Return("26L"));

                // Setup the second element

                ON_CALL(*element2, Airport()).WillByDefault(Return("EGKK - London Gatwick"));

                ON_CALL(*element2, Name()).WillByDefault(Return("EGKK London Gatwick 08L - 26R"));

                ON_CALL(*element2, Runway1ActiveForDepartures()).WillByDefault(Return(false));

                ON_CALL(*element2, Runway1ActiveForArrivals()).WillByDefault(Return(false));

                ON_CALL(*element2, Runway2ActiveForDepartures()).WillByDefault(Return(false));

                ON_CALL(*element2, Runway2ActiveForArrivals()).WillByDefault(Return(false));

                ON_CALL(*element2, Runway1Heading()).WillByDefault(Return(81));

                ON_CALL(*element2, Runway2Heading()).WillByDefault(Return(259));

                ON_CALL(*element2, Runway1Identifier()).WillByDefault(Return("08L"));

                ON_CALL(*element2, Runway2Identifier()).WillByDefault(Return("26R"));

                // Setup the fake element to test robustness
                ON_CALL(*element3, Airport()).WillByDefault(Return("EGKK"));

                // Setup the fouth element, a duplicate of the second with different runways setup
                ON_CALL(*element2, Airport()).WillByDefault(Return("EGKK - London Gatwick"));

                ON_CALL(*element2, Name()).WillByDefault(Return("EGKK London Gatwick 08L - 26R"));

                ON_CALL(*element2, Runway1ActiveForDepartures()).WillByDefault(Return(true));

                ON_CALL(*element2, Runway1ActiveForArrivals()).WillByDefault(Return(true));

                ON_CALL(*element2, Runway2ActiveForDepartures()).WillByDefault(Return(false));

                ON_CALL(*element2, Runway2ActiveForArrivals()).WillByDefault(Return(false));

                ON_CALL(*element2, Runway1Heading()).WillByDefault(Return(81));

                ON_CALL(*element2, Runway2Heading()).WillByDefault(Return(259));

                ON_CALL(*element2, Runway1Identifier()).WillByDefault(Return("08L"));

                ON_CALL(*element2, Runway2Identifier()).WillByDefault(Return("26R"));

                std::set<std::shared_ptr<EuroscopeSectorFileElementInterface>> elements = {
                    element1, element2, element3, element4};

                ON_CALL(mockSectorFile, GetAllElementsByType(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY))
                    .WillByDefault(Return(elements));
            }

            std::shared_ptr<NiceMock<MockEuroscopeSectorFileElementInterface>> element1;
            std::shared_ptr<NiceMock<MockEuroscopeSectorFileElementInterface>> element2;
            std::shared_ptr<NiceMock<MockEuroscopeSectorFileElementInterface>> element3;
            std::shared_ptr<NiceMock<MockEuroscopeSectorFileElementInterface>> element4;

            NiceMock<MockSectorFileProviderInterface> mockSectorFile;
            NiceMock<MockUserSettingProviderInterface> mockUserSettings;
            UserSetting userSetting;
            RunwayCollection collection;
        };

        TEST_F(RunwayCollectionTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, collection.Count());
        }

        TEST_F(RunwayCollectionTest, ItAddsRunwaysOnAsrLoad)
        {
            this->SetUpDefaultElements();
            this->collection.AsrLoadedEvent(this->userSetting);
            EXPECT_EQ(4, collection.Count());
        }

        TEST_F(RunwayCollectionTest, ItLoadsRunwayData)
        {
            this->SetUpDefaultElements();
            this->collection.AsrLoadedEvent(this->userSetting);

            const Runway& runway08R = this->collection.FetchByIdentifierAndAirfield("08R", "EGKK");
            EXPECT_EQ("EGKK", runway08R.airfield);
            EXPECT_EQ(81, runway08R.heading);
            EXPECT_EQ("08R", runway08R.identifier);
            EXPECT_FALSE(runway08R.ActiveForDepartures());
            EXPECT_FALSE(runway08R.ActiveForArrivals());

            const Runway& runway26L = this->collection.FetchByIdentifierAndAirfield("26L", "EGKK");
            EXPECT_EQ("EGKK", runway26L.airfield);
            EXPECT_EQ(259, runway26L.heading);
            EXPECT_EQ("26L", runway26L.identifier);
            EXPECT_TRUE(runway26L.ActiveForDepartures());
            EXPECT_TRUE(runway26L.ActiveForArrivals());

            const Runway& runway26R = this->collection.FetchByIdentifierAndAirfield("26R", "EGKK");
            EXPECT_EQ("EGKK", runway26R.airfield);
            EXPECT_EQ(259, runway26R.heading);
            EXPECT_EQ("26R", runway26R.identifier);
            EXPECT_FALSE(runway26R.ActiveForDepartures());
            EXPECT_FALSE(runway26R.ActiveForArrivals());

            const Runway& runway08L = this->collection.FetchByIdentifierAndAirfield("08L", "EGKK");
            EXPECT_EQ("EGKK", runway08L.airfield);
            EXPECT_EQ(81, runway08L.heading);
            EXPECT_EQ("08L", runway08L.identifier);
            EXPECT_TRUE(runway08L.ActiveForDepartures());
            EXPECT_TRUE(runway08L.ActiveForArrivals());
        }

        TEST_F(RunwayCollectionTest, ItReturnsInvalidIfNotFoundByAirfield)
        {
            this->SetUpDefaultElements();
            this->collection.AsrLoadedEvent(this->userSetting);
            EXPECT_EQ(this->collection.invalidRunway, this->collection.FetchByIdentifierAndAirfield("27L", "EGKK"));
        }

        TEST_F(RunwayCollectionTest, ItUpdatesRunwayActivity)
        {
            std::shared_ptr<NiceMock<MockEuroscopeSectorFileElementInterface>> element =
                std::make_shared<NiceMock<MockEuroscopeSectorFileElementInterface>>();
            std::set<std::shared_ptr<EuroscopeSectorFileElementInterface>> elementList = {element};

            ON_CALL(mockSectorFile, GetAllElementsByType(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY))
                .WillByDefault(Return(elementList));

            ON_CALL(*element, Airport()).WillByDefault(Return("EGKK - London Gatwick"));

            ON_CALL(*element, Name()).WillByDefault(Return("EGKK London Gatwick 08R - 26L"));

            EXPECT_CALL(*element, Runway1ActiveForDepartures()).Times(2).WillOnce(Return(true)).WillOnce(Return(false));

            EXPECT_CALL(*element, Runway1ActiveForArrivals()).Times(2).WillOnce(Return(true)).WillOnce(Return(false));

            EXPECT_CALL(*element, Runway2ActiveForDepartures()).Times(2).WillOnce(Return(false)).WillOnce(Return(true));

            EXPECT_CALL(*element, Runway2ActiveForArrivals()).Times(2).WillOnce(Return(false)).WillOnce(Return(true));

            ON_CALL(*element, Runway1ActiveForDepartures()).WillByDefault(Return(false));

            ON_CALL(*element, Runway1ActiveForArrivals()).WillByDefault(Return(false));

            ON_CALL(*element, Runway2ActiveForDepartures()).WillByDefault(Return(true));

            ON_CALL(*element, Runway2ActiveForArrivals()).WillByDefault(Return(true));

            ON_CALL(*element, Runway1Heading()).WillByDefault(Return(81));

            ON_CALL(*element, Runway2Heading()).WillByDefault(Return(259));

            ON_CALL(*element, Runway1Identifier()).WillByDefault(Return("08R"));

            ON_CALL(*element, Runway2Identifier()).WillByDefault(Return("26L"));

            this->collection.AsrLoadedEvent(this->userSetting);

            const Runway& runway26L = this->collection.FetchByIdentifierAndAirfield("26L", "EGKK");
            EXPECT_EQ("EGKK", runway26L.airfield);
            EXPECT_EQ(259, runway26L.heading);
            EXPECT_EQ("26L", runway26L.identifier);
            EXPECT_FALSE(runway26L.ActiveForDepartures());
            EXPECT_FALSE(runway26L.ActiveForArrivals());

            const Runway& runway08R = this->collection.FetchByIdentifierAndAirfield("08R", "EGKK");
            EXPECT_EQ("EGKK", runway08R.airfield);
            EXPECT_EQ(81, runway08R.heading);
            EXPECT_EQ("08R", runway08R.identifier);
            EXPECT_TRUE(runway08R.ActiveForDepartures());
            EXPECT_TRUE(runway08R.ActiveForArrivals());

            // Change the runways
            this->collection.RunwayDialogSaved();

            EXPECT_FALSE(runway08R.ActiveForDepartures());
            EXPECT_FALSE(runway08R.ActiveForArrivals());
            EXPECT_TRUE(runway26L.ActiveForDepartures());
            EXPECT_TRUE(runway26L.ActiveForArrivals());
        }
    } // namespace SectorFile
} // namespace UKControllerPluginTest
