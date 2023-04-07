#include "wake/WakeCategoryMapperCollection.h"
#include "wake/WakeCategoryMapperCollectionFactory.h"
#include "wake/WakeScheme.h"
#include "wake/WakeSchemeCollection.h"

using UKControllerPlugin::Wake::MakeMapperCollection;
using UKControllerPlugin::Wake::WakeCategory;
using UKControllerPlugin::Wake::WakeScheme;
using UKControllerPlugin::Wake::WakeSchemeCollection;

namespace UKControllerPluginTest::Wake {
    class WakeCategoryMapperCollectionFactoryTest : public testing::Test
    {
        public:
        WakeCategoryMapperCollectionFactoryTest()
            : scheme1(std::make_shared<WakeScheme>(1, "UK", "UK", std::list<std::shared_ptr<WakeCategory>>{})),
              scheme2(std::make_shared<WakeScheme>(2, "RECAT", "RECAT-EU", std::list<std::shared_ptr<WakeCategory>>{}))
        {
            collection.Add(scheme1);
            collection.Add(scheme2);
        }

        testing::NiceMock<Aircraft::MockAircraftTypeMapper> typeMapper;
        std::shared_ptr<WakeScheme> scheme1;
        std::shared_ptr<WakeScheme> scheme2;
        WakeSchemeCollection collection;
    };

    TEST_F(WakeCategoryMapperCollectionFactoryTest, ItBuildsTheCollection)
    {
        auto mapperCollection = MakeMapperCollection(collection, typeMapper);
        EXPECT_EQ(2, mapperCollection->Count());
        EXPECT_NE(nullptr, mapperCollection->Get(1));
        EXPECT_NE(nullptr, mapperCollection->Get(2));
    }
} // namespace UKControllerPluginTest::Wake
