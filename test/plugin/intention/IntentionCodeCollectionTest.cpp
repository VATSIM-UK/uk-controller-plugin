#include "intention/IntentionCodeCollection.h"
#include "intention/IntentionCodeMetadata.h"
#include "intention/IntentionCodeModel.h"
#include "intention/SingleCode.h"

using UKControllerPlugin::IntentionCode::IntentionCodeCollection;
using UKControllerPlugin::IntentionCode::IntentionCodeMetadata;
using UKControllerPlugin::IntentionCode::IntentionCodeModel;
using UKControllerPlugin::IntentionCode::SingleCode;

namespace UKControllerPluginTest::IntentionCode {
    class IntentionCodeCollectionTest : public testing::Test
    {
        public:
        IntentionCodeCollectionTest()
            : model1(std::make_shared<IntentionCodeModel>(
                  1, std::make_unique<SingleCode>("AF"), std::make_unique<testing::NiceMock<MockCondition>>(),
                  std::unique_ptr<IntentionCodeMetadata>(new IntentionCodeMetadata))),
              model2(std::make_shared<IntentionCodeModel>(
                  2, std::make_unique<SingleCode>("AF"), std::make_unique<testing::NiceMock<MockCondition>>(), std::unique_ptr<IntentionCodeMetadata>(new IntentionCodeMetadata))),
              model3(std::make_shared<IntentionCodeModel>(
                  3, std::make_unique<SingleCode>("AF"), std::make_unique<testing::NiceMock<MockCondition>>(), std::unique_ptr<IntentionCodeMetadata>(new IntentionCodeMetadata)))
        {
        }

        std::shared_ptr<IntentionCodeModel> model1;
        std::shared_ptr<IntentionCodeModel> model2;
        std::shared_ptr<IntentionCodeModel> model3;
        IntentionCodeCollection collection;
    };

    TEST_F(IntentionCodeCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(IntentionCodeCollectionTest, ItAddsModels)
    {
        collection.Add(model1);
        collection.Add(model2);
        collection.Add(model3);
        EXPECT_EQ(3, collection.Count());
    }

    TEST_F(IntentionCodeCollectionTest, ItDoesntAddDuplicateModels)
    {
        collection.Add(model1);
        collection.Add(model1);
        collection.Add(model2);
        collection.Add(model2);
        collection.Add(model3);
        collection.Add(model3);
        EXPECT_EQ(3, collection.Count());
    }

    TEST_F(IntentionCodeCollectionTest, FindByIdReturnsModel)
    {
        collection.Add(model1);
        collection.Add(model2);
        collection.Add(model3);
        EXPECT_EQ(model2, collection.FindById(2));
    }

    TEST_F(IntentionCodeCollectionTest, FindByIdReturnsNullptrIfNoModel)
    {
        collection.Add(model1);
        collection.Add(model2);
        collection.Add(model3);
        EXPECT_EQ(nullptr, collection.FindById(5));
    }

    TEST_F(IntentionCodeCollectionTest, FirstWhereIteratesInInsertionOrder)
    {
        collection.Add(model3);
        collection.Add(model1);
        collection.Add(model2);

        std::list<int> modelIds;

        static_cast<void>(collection.FirstWhere([&modelIds](const IntentionCodeModel& model) -> bool {
            modelIds.push_back(model.Id());
            return false;
        }));

        EXPECT_EQ(std::list<int>({3, 1, 2}), modelIds);
    }

    TEST_F(IntentionCodeCollectionTest, FirstWhereReturnsNullptrIfNothingFound)
    {
        collection.Add(model3);
        collection.Add(model1);
        collection.Add(model2);

        const auto foundModel =
            collection.FirstWhere([](const IntentionCodeModel& model) -> bool { return model.Id() == 5; });

        EXPECT_EQ(nullptr, foundModel);
    }

    TEST_F(IntentionCodeCollectionTest, FirstWhereReturnsFoundModel)
    {
        collection.Add(model3);
        collection.Add(model1);
        collection.Add(model2);

        const auto foundModel =
            collection.FirstWhere([](const IntentionCodeModel& model) -> bool { return model.Id() == 1; });

        EXPECT_EQ(model1, foundModel);
    }
} // namespace UKControllerPluginTest::IntentionCode
