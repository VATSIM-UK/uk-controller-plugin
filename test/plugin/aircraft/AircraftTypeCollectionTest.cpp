#include "aircraft/AircraftType.h"
#include "aircraft/AircraftTypeCollection.h"

using UKControllerPlugin::Aircraft::AircraftType;
using UKControllerPlugin::Aircraft::AircraftTypeCollection;

namespace UKControllerPluginTest::Aircraft {
    class AircraftTypeCollectionTest : public testing::Test
    {
        public:
        AircraftTypeCollectionTest()
            : type1(std::make_shared<AircraftType>(1, "B738", std::set<int>{})),
              type2(std::make_shared<AircraftType>(2, "A320", std::set<int>{}))
        {
        }

        std::shared_ptr<AircraftType> type1;
        std::shared_ptr<AircraftType> type2;
        AircraftTypeCollection collection;
    };

    TEST_F(AircraftTypeCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(AircraftTypeCollectionTest, ItAddsTypes)
    {
        collection.Add(type1);
        collection.Add(type2);
        EXPECT_EQ(2, collection.Count());
    }

    TEST_F(AircraftTypeCollectionTest, ItDoesntAddDuplicateTypes)
    {
        collection.Add(type1);
        collection.Add(type1);
        collection.Add(type1);
        collection.Add(type2);
        collection.Add(type2);
        collection.Add(type2);
        EXPECT_EQ(2, collection.Count());
    }

    TEST_F(AircraftTypeCollectionTest, ItGetsTypesByIcaoCode)
    {
        collection.Add(type1);
        collection.Add(type2);
        EXPECT_EQ(type1, collection.GetByIcaoCode("B738"));
    }

    TEST_F(AircraftTypeCollectionTest, ItReturnsNullptrIfTypeByIcaoCodeNotFound)
    {
        collection.Add(type1);
        collection.Add(type2);
        EXPECT_EQ(nullptr, collection.GetByIcaoCode("somecode"));
    }
} // namespace UKControllerPluginTest::Aircraft
