#include "pch/pch.h"
#include "euroscope/UserSectorFileTracker.h"
#include "mock/MockSectorFileProviderInterface.h"
#include "mock/MockSectorFileAwareInterface.h"
#include "euroscope/SectorFileAwareCollection.h"

using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Ref;
using UKControllerPlugin::Euroscope::UserSectorFileTracker;
using UKControllerPluginTest::Euroscope::MockSectorFileAwareInterface;
using UKControllerPluginTest::Euroscope::MockSectorFileProviderInterface;


namespace UKControllerPlugin {
    namespace Euroscope {

        class UserSectorFileTrackerTest : public Test
        {
            public:

                UserSectorFileTrackerTest()
                    : tracker(provider, collection)
                {
                    mockHandler.reset(new NiceMock<MockSectorFileAwareInterface>);
                    this->collection.AddHandler(mockHandler);
                }

                std::shared_ptr<NiceMock<MockSectorFileAwareInterface>> mockHandler;
                NiceMock<MockSectorFileProviderInterface> provider;
                SectorFileAwareCollection collection;
                UserSectorFileTracker tracker;
        };

        TEST_F(UserSectorFileTrackerTest, ItStartsWithNoLoadedSectorFile)
        {
            EXPECT_EQ("", this->tracker.LoadedSectorFile());
        }

        TEST_F(UserSectorFileTrackerTest, LoadingTheFirstSectorFileChangesTheSectorFile)
        {
            ON_CALL(this->provider, GetSectorFileName())
                .WillByDefault(Return("sector1"));

            this->tracker.TimedEventTrigger();

            EXPECT_EQ("sector1", this->tracker.LoadedSectorFile());
        }

        TEST_F(UserSectorFileTrackerTest, LoadingANewSectorFileChangesTheSectorFile)
        {
            EXPECT_CALL(this->provider, GetSectorFileName())
                .Times(4)
                .WillOnce(Return("sector1"))
                .WillOnce(Return("sector1"))
                .WillOnce(Return("sector2"))
                .WillOnce(Return("sector2"));

            this->tracker.TimedEventTrigger();
            this->tracker.TimedEventTrigger();

            EXPECT_EQ("sector2", this->tracker.LoadedSectorFile());
        }

        TEST_F(UserSectorFileTrackerTest, ChangingTheSectorFileTriggersUpdate)
        {
            ON_CALL(this->provider, GetSectorFileName())
                .WillByDefault(Return("sector1"));

            EXPECT_CALL(*this->mockHandler, SectorFileUpdate(Ref(this->provider)))
                .Times(1);

            this->tracker.TimedEventTrigger();
        }

        TEST_F(UserSectorFileTrackerTest, NoUpdateHappensIfSameSectorFile)
        {
            ON_CALL(this->provider, GetSectorFileName())
                .WillByDefault(Return("sector1"));

            EXPECT_CALL(*this->mockHandler, SectorFileUpdate(Ref(this->provider)))
                .Times(1);

            this->tracker.TimedEventTrigger();
            this->tracker.TimedEventTrigger();
            this->tracker.TimedEventTrigger();
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
