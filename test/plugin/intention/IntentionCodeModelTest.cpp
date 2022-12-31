#include "intention/IntentionCodeModel.h"
#include "intention/SingleCode.h"

using UKControllerPlugin::IntentionCode::IntentionCodeModel;
using UKControllerPlugin::IntentionCode::SingleCode;

namespace UKControllerPluginTest::IntentionCode {
    class IntentionCodeModelTest : public testing::Test
    {
        public:
        IntentionCodeModelTest()
            : condition(std::make_unique<testing::NiceMock<MockCondition>>()), code(std::make_unique<SingleCode>("AF"))
        {
        }

        std::unique_ptr<testing::NiceMock<MockCondition>> condition;
        std::unique_ptr<SingleCode> code;
    };

    TEST_F(IntentionCodeModelTest, ItReturnsAnId)
    {
        IntentionCodeModel model(1, std::move(code), std::move(condition));
        EXPECT_EQ(1, model.Id());
    }

    TEST_F(IntentionCodeModelTest, ItHasACodeGenerator)
    {
        const auto codeGeneratorRawPointer = code.get();
        IntentionCodeModel model(1, std::move(code), std::move(condition));
        EXPECT_EQ(codeGeneratorRawPointer, &model.Generator());
    }

    TEST_F(IntentionCodeModelTest, ItHasACondition)
    {
        const auto conditionRawPointer = condition.get();
        IntentionCodeModel model(1, std::move(code), std::move(condition));
        EXPECT_EQ(conditionRawPointer, &model.Conditions());
    }
} // namespace UKControllerPluginTest::IntentionCode
