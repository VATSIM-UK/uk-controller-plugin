#include "navaids/Navaid.h"
#include "navaids/NavaidCollection.h"

using ::testing::Test;
using UKControllerPlugin::Navaids::Navaid;
using UKControllerPlugin::Navaids::NavaidCollection;

namespace UKControllerPluginTest {
    namespace Navaids {

        class NavaidCollectionTest : public Test
        {
            public:
            Navaid navaid1 = {1, "TIMBA"};
            Navaid navaid2 = {2, "WILLO"};
            NavaidCollection collection;
        };

        TEST_F(NavaidCollectionTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->collection.Count());
        }

        TEST_F(NavaidCollectionTest, ItAddsNavaids)
        {
            this->collection.AddNavaid(navaid1);
            this->collection.AddNavaid(navaid2);
            EXPECT_EQ(2, this->collection.Count());
        }

        TEST_F(NavaidCollectionTest, ItDoesntAddDuplicateNavaids)
        {
            this->collection.AddNavaid(navaid1);
            this->collection.AddNavaid(navaid1);
            this->collection.AddNavaid(navaid2);
            this->collection.AddNavaid(navaid2);
            EXPECT_EQ(2, this->collection.Count());
        }

        TEST_F(NavaidCollectionTest, ItFindsNavaidsByIdentifier)
        {
            this->collection.AddNavaid(navaid1);
            EXPECT_EQ(navaid1, this->collection.GetByIdentifier("TIMBA"));
        }

        TEST_F(NavaidCollectionTest, ItReturnsInvalidIfNavaidNotFound)
        {
            this->collection.AddNavaid(navaid1);
            EXPECT_EQ(this->collection.invalidNavaid, this->collection.GetByIdentifier("WILLO"));
        }

        TEST_F(NavaidCollectionTest, ItFindsNavaidsById)
        {
            this->collection.AddNavaid(navaid1);
            EXPECT_EQ(navaid1, this->collection.Get(1));
        }

        TEST_F(NavaidCollectionTest, ItReturnsInvalidIfNavaidNotFoundById)
        {
            this->collection.AddNavaid(navaid1);
            EXPECT_EQ(this->collection.invalidNavaid, this->collection.Get(33));
        }
    } // namespace Navaids
} // namespace UKControllerPluginTest
