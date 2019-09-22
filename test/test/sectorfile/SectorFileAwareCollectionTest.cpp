#include "pch/pch.h"
#include "sectorfile/SectorFileAwareCollection.h"
#include "mock/MockSectorFileAwareInterface.h"
#include "mock/MockSectorFileProviderInterface.h"

using UKControllerPlugin::SectorFile::SectorFileAwareCollection;
using UKControllerPluginTest::SectorFile::MockSectorFileProviderInterface;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Ref;

namespace UKControllerPluginTest {
    namespace SectorFile {

        class SectorFileAwareCollectionTest : public Test
        {
            public:

                void SetUp()
                {
                    mockHandler1.reset(new NiceMock<MockSectorFileAwareInterface>);
                    mockHandler2.reset(new NiceMock<MockSectorFileAwareInterface>);
                }
           
                std::shared_ptr<NiceMock<MockSectorFileAwareInterface>> mockHandler1;
                std::shared_ptr<NiceMock<MockSectorFileAwareInterface>> mockHandler2;
                NiceMock<MockSectorFileProviderInterface> provider;
                SectorFileAwareCollection collection;
        };

        TEST_F(SectorFileAwareCollectionTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->collection.CountHandlers());
        }

        TEST_F(SectorFileAwareCollectionTest, ItAddsHandlers)
        {
            this->collection.AddHandler(mockHandler1);
            this->collection.AddHandler(mockHandler2);
            EXPECT_EQ(2, this->collection.CountHandlers());
        }

        TEST_F(SectorFileAwareCollectionTest, ItDoesntAddDuplicateHandlers)
        {
            this->collection.AddHandler(mockHandler1);
            this->collection.AddHandler(mockHandler1);
            EXPECT_EQ(1, this->collection.CountHandlers());
        }

        TEST_F(SectorFileAwareCollectionTest, ItPassesEventsToHandlers)
        {
            this->collection.AddHandler(mockHandler1);
            this->collection.AddHandler(mockHandler2);

            EXPECT_CALL(*this->mockHandler1, SectorFileUpdate(Ref(this->provider)))
                .Times(1);

            EXPECT_CALL(*this->mockHandler2, SectorFileUpdate(Ref(this->provider)))
                .Times(1);


            this->collection.SectorFileUpdate(this->provider);
        }
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
