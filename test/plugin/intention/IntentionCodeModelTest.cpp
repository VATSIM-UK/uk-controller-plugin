#include "intention/IntentionCodeMetadata.h"
#include "intention/IntentionCodeModel.h"
#include "intention/SingleCode.h"

using UKControllerPlugin::IntentionCode::IntentionCodeMetadata;
using UKControllerPlugin::IntentionCode::IntentionCodeModel;
using UKControllerPlugin::IntentionCode::SingleCode;

namespace UKControllerPluginTest::IntentionCode {
    class IntentionCodeModelTest : public testing::Test
    {
        public:
        IntentionCodeModelTest()
            : condition(std::make_unique<testing::NiceMock<MockCondition>>()), code(std::make_unique<SingleCode>("AF")),
              metadata(std::unique_ptr<IntentionCodeMetadata>(new IntentionCodeMetadata))
        {
        }

        std::unique_ptr<testing::NiceMock<MockCondition>> condition;
        std::unique_ptr<SingleCode> code;
        std::unique_ptr<IntentionCodeMetadata> metadata;
    };

    TEST_F(IntentionCodeModelTest, ItReturnsAnId)
    {
        IntentionCodeModel model(1, std::move(code), std::move(condition), std::move(metadata));
        EXPECT_EQ(1, model.Id());
    }

    TEST_F(IntentionCodeModelTest, ItHasACodeGenerator)
    {
        const auto codeGeneratorRawPointer = code.get();
        IntentionCodeModel model(1, std::move(code), std::move(condition), std::move(metadata));
        EXPECT_EQ(codeGeneratorRawPointer, &model.Generator());
    }

    TEST_F(IntentionCodeModelTest, ItHasACondition)
    {
        const auto conditionRawPointer = condition.get();
        IntentionCodeModel model(1, std::move(code), std::move(condition), std::move(metadata));
        EXPECT_EQ(conditionRawPointer, &model.Conditions());
    }

    TEST_F(IntentionCodeModelTest, ItHasMetadata)
    {
        const auto metadataRawPointer = metadata.get();
        IntentionCodeModel model(1, std::move(code), std::move(condition), std::move(metadata));
        EXPECT_EQ(metadataRawPointer, &model.Metadata());
    }
} // namespace UKControllerPluginTest::IntentionCode
