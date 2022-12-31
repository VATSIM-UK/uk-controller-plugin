#include "intention/FirExitPoint.h"
#include "intention/FirExitPointCollection.h"

using UKControllerPlugin::IntentionCode::FirExitPoint;
using UKControllerPlugin::IntentionCode::FirExitPointCollection;

namespace UKControllerPluginTest::IntentionCode {
    class FirExitPointCollectionTest : public testing::Test
    {
        public:
        FirExitPointCollectionTest()
            : exitDetermination(std::make_shared<testing::NiceMock<MockExitDetermination>>()),
              point1(std::make_shared<FirExitPoint>(1, "FOO", false, exitDetermination)),
              point2(std::make_shared<FirExitPoint>(2, "WOO", false, exitDetermination)),
              point3(std::make_shared<FirExitPoint>(3, "DOO", false, exitDetermination))
        {
        }

        std::shared_ptr<testing::NiceMock<MockExitDetermination>> exitDetermination;
        std::shared_ptr<FirExitPoint> point1;
        std::shared_ptr<FirExitPoint> point2;
        std::shared_ptr<FirExitPoint> point3;
        FirExitPointCollection collection;
    };

    TEST_F(FirExitPointCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.CountPoints());
    }

    TEST_F(FirExitPointCollectionTest, ItAddsPoints)
    {
        collection.Add(point1);
        collection.Add(point2);
        collection.Add(point3);
        EXPECT_EQ(3, collection.CountPoints());
    }

    TEST_F(FirExitPointCollectionTest, ItDoesntAddDuplicatePoints)
    {
        collection.Add(point1);
        collection.Add(point1);
        collection.Add(point2);
        collection.Add(point2);
        collection.Add(point3);
        collection.Add(point3);
        EXPECT_EQ(3, collection.CountPoints());
    }

    TEST_F(FirExitPointCollectionTest, ItReturnsNullptrIfPointNotFoundByIdentifier)
    {
        collection.Add(point1);
        collection.Add(point2);
        collection.Add(point3);
        EXPECT_EQ(nullptr, collection.PointByIdentifier("NOPE"));
    }

    TEST_F(FirExitPointCollectionTest, ItReturnsPointIfFoundByIdentifier)
    {
        collection.Add(point1);
        collection.Add(point2);
        collection.Add(point3);
        EXPECT_EQ(point1, collection.PointByIdentifier("FOO"));
    }

    TEST_F(FirExitPointCollectionTest, ItReturnsNullptrIfPointNotFoundById)
    {
        collection.Add(point1);
        collection.Add(point2);
        collection.Add(point3);
        EXPECT_EQ(nullptr, collection.PointById(55));
    }

    TEST_F(FirExitPointCollectionTest, ItReturnsPointIfFoundById)
    {
        collection.Add(point1);
        collection.Add(point2);
        collection.Add(point3);
        EXPECT_EQ(point1, collection.PointById(1));
    }
} // namespace UKControllerPluginTest::IntentionCode
