#include "pch/pch.h"
#include "euroscope/RunwayDialogAwareCollection.h"
#include "mock/MockRunwayDialogAwareInterface.h"

using UKControllerPlugin::Euroscope::RunwayDialogAwareCollection;
using UKControllerPluginTest::Euroscope::MockRunwayDialogAwareInterface;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Ref;

namespace UKControllerPluginTest {
    namespace Euroscope {

        class RunwayDialogAwareCollectionTest : public Test
        {
            public:

                void SetUp()
                {
                    mockHandler1.reset(new NiceMock<MockRunwayDialogAwareInterface>);
                    mockHandler2.reset(new NiceMock<MockRunwayDialogAwareInterface>);
                }
           
                std::shared_ptr<NiceMock<MockRunwayDialogAwareInterface>> mockHandler1;
                std::shared_ptr<NiceMock<MockRunwayDialogAwareInterface>> mockHandler2;
                RunwayDialogAwareCollection collection;
        };

        TEST_F(RunwayDialogAwareCollectionTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->collection.CountHandlers());
        }

        TEST_F(RunwayDialogAwareCollectionTest, ItAddsHandlers)
        {
            this->collection.AddHandler(mockHandler1);
            this->collection.AddHandler(mockHandler2);
            EXPECT_EQ(2, this->collection.CountHandlers());
        }

        TEST_F(RunwayDialogAwareCollectionTest, ItDoesntAddDuplicateHandlers)
        {
            this->collection.AddHandler(mockHandler1);
            this->collection.AddHandler(mockHandler1);
            EXPECT_EQ(1, this->collection.CountHandlers());
        }

        TEST_F(RunwayDialogAwareCollectionTest, ItPassesEventsToHandlers)
        {
            this->collection.AddHandler(mockHandler1);
            this->collection.AddHandler(mockHandler2);

            EXPECT_CALL(*this->mockHandler1, RunwayDialogSaved())
                .Times(1);

            EXPECT_CALL(*this->mockHandler2, RunwayDialogSaved())
                .Times(1);


            this->collection.RunwayDialogSaved();
        }
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
