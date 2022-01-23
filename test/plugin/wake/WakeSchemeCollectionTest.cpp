#include "wake/WakeScheme.h"
#include "wake/WakeSchemeCollection.h"

using UKControllerPlugin::Wake::WakeCategory;
using UKControllerPlugin::Wake::WakeScheme;
using UKControllerPlugin::Wake::WakeSchemeCollection;

namespace UKControllerPluginTest::Wake {
    class WakeSchemeCollectionTest : public testing::Test
    {
        public:
        WakeSchemeCollectionTest()
            : scheme1(std::make_shared<WakeScheme>(1, "UK", "UK", std::list<std::shared_ptr<WakeCategory>>{})),
              scheme2(std::make_shared<WakeScheme>(2, "RECAT", "RECAT-EU", std::list<std::shared_ptr<WakeCategory>>{}))
        {
        }

        std::shared_ptr<WakeScheme> scheme1;
        std::shared_ptr<WakeScheme> scheme2;
        WakeSchemeCollection collection;
    };

    TEST_F(WakeSchemeCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(WakeSchemeCollectionTest, ItAddsSchemes)
    {
        collection.Add(scheme1);
        collection.Add(scheme2);
        EXPECT_EQ(2, collection.Count());
    }

    TEST_F(WakeSchemeCollectionTest, ItDoesntAddDuplicateSchemes)
    {
        collection.Add(scheme1);
        collection.Add(scheme1);
        collection.Add(scheme1);
        collection.Add(scheme2);
        collection.Add(scheme2);
        collection.Add(scheme2);
        EXPECT_EQ(2, collection.Count());
    }

    TEST_F(WakeSchemeCollectionTest, ItGetsSchemesByKey)
    {
        collection.Add(scheme1);
        collection.Add(scheme2);
        EXPECT_EQ(scheme2, collection.GetByKey("RECAT"));
    }

    TEST_F(WakeSchemeCollectionTest, ItReturnsNullptrIfSchemeByKeyNotFound)
    {
        collection.Add(scheme1);
        collection.Add(scheme2);
        EXPECT_EQ(nullptr, collection.GetByKey("somekey"));
    }
} // namespace UKControllerPluginTest::Wake
