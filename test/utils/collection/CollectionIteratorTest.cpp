#include "collection/CollectionIterator.h"

namespace UKControllerPluginUtilsTest::Collection {
    class CollectionIteratorTest : public testing::Test
    {
        public:
        CollectionIteratorTest()
            : item1(std::make_shared<std::string>("item1")), item2(std::make_shared<std::string>("item2")),
              item3(std::make_shared<std::string>("item3")), map({{1, item1}, {2, item2}, {3, item3}})
        {
        }

        [[nodiscard]] auto GetIterator() -> UKControllerPluginUtils::Collection::
            CollectionIterator<int, std::string, std::map<int, std::shared_ptr<std::string>>::const_iterator>
        {
            return UKControllerPluginUtils::Collection::
                CollectionIterator<int, std::string, std::map<int, std::shared_ptr<std::string>>::const_iterator>(
                    mutex, map.cbegin());
        }

        std::shared_ptr<std::string> item1;
        std::shared_ptr<std::string> item2;
        std::shared_ptr<std::string> item3;
        std::recursive_mutex mutex;
        std::map<int, std::shared_ptr<std::string>> map;
    };

    TEST_F(CollectionIteratorTest, ItReturnsTheCurrentValue)
    {
        EXPECT_EQ("item1", *GetIterator());
    }

    TEST_F(CollectionIteratorTest, ItHasPointerOperator)
    {
        EXPECT_EQ("item1", std::string(GetIterator()->c_str()));
    }

    TEST_F(CollectionIteratorTest, ItHasPostfixIncrement)
    {
        auto iterator = GetIterator();

        EXPECT_EQ("item1", *(iterator++));
        EXPECT_EQ("item2", *(iterator));
    }

    TEST_F(CollectionIteratorTest, ItHasPrefixIncrement)
    {
        auto iterator = GetIterator();

        EXPECT_EQ("item2", *(++iterator));
        EXPECT_EQ("item2", *(iterator));
    }

    TEST_F(CollectionIteratorTest, ItHasEquality)
    {
        std::recursive_mutex otherMutex;
        auto iterator = GetIterator();
        auto iterator2 = UKControllerPluginUtils::Collection::
            CollectionIterator<int, std::string, std::map<int, std::shared_ptr<std::string>>::const_iterator>(
                otherMutex, map.cbegin());

        EXPECT_TRUE(iterator == iterator2);
    }

    TEST_F(CollectionIteratorTest, ItDoesntHaveEquality)
    {
        std::recursive_mutex otherMutex;
        auto iterator = GetIterator();
        iterator++;
        auto iterator2 = UKControllerPluginUtils::Collection::
            CollectionIterator<int, std::string, std::map<int, std::shared_ptr<std::string>>::const_iterator>(
                otherMutex, map.cbegin());

        EXPECT_FALSE(iterator == iterator2);
    }

    TEST_F(CollectionIteratorTest, ItHasInequality)
    {
        std::recursive_mutex otherMutex;
        auto iterator = GetIterator();
        auto iterator2 = UKControllerPluginUtils::Collection::
            CollectionIterator<int, std::string, std::map<int, std::shared_ptr<std::string>>::const_iterator>(
                otherMutex, map.cbegin());
        iterator++;

        EXPECT_TRUE(iterator != iterator2);
    }

    TEST_F(CollectionIteratorTest, ItDoesntHaveInequality)
    {
        std::recursive_mutex otherMutex;
        auto iterator = GetIterator();
        auto iterator2 = UKControllerPluginUtils::Collection::
            CollectionIterator<int, std::string, std::map<int, std::shared_ptr<std::string>>::const_iterator>(
                otherMutex, map.cbegin());

        EXPECT_FALSE(iterator != iterator2);
    }
} // namespace UKControllerPluginUtilsTest::Collection
