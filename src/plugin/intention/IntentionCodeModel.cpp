#include "CodeGenerator.h"
#include "Condition.h"
#include "IntentionCodeModel.h"

namespace UKControllerPlugin::IntentionCode {

    IntentionCodeModel::IntentionCodeModel(
        int id, std::unique_ptr<CodeGenerator> codeGenerator, std::unique_ptr<Condition> condition)
        : id(id), codeGenerator(std::move(codeGenerator)), condition(std::move(condition))
    {
        assert(!codeGenerator && "Code generator not set for intention code");
        assert(!condition && "Condition not set for intention code");
    }
    IntentionCodeModel::~IntentionCodeModel() = default;

    auto IntentionCodeModel::Id() const -> int
    {
        return id;
    }

    auto IntentionCodeModel::Generator() const -> const CodeGenerator&
    {
        return *codeGenerator;
    }

    auto IntentionCodeModel::Conditions() const -> const Condition&
    {
        return *condition;
    }
} // namespace UKControllerPlugin::IntentionCode
