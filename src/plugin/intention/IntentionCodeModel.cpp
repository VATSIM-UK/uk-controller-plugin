#include "CodeGenerator.h"
#include "Condition.h"
#include "IntentionCodeMetadata.h"
#include "IntentionCodeModel.h"

namespace UKControllerPlugin::IntentionCode {

    IntentionCodeModel::IntentionCodeModel(
        int id,
        std::unique_ptr<CodeGenerator> codeGenerator,
        std::unique_ptr<Condition> condition,
        std::unique_ptr<IntentionCodeMetadata> metadata)
        : id(id), codeGenerator(std::move(codeGenerator)), condition(std::move(condition)),
          metadata(std::move(metadata))
    {
        assert(this->codeGenerator && "Code generator not set for intention code");
        assert(this->condition && "Condition not set for intention code");
        assert(this->metadata && "Metadata not set for intention code");
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

    auto IntentionCodeModel::Conditions() const -> Condition&
    {
        return *condition;
    }

    auto IntentionCodeModel::Metadata() const -> const IntentionCodeMetadata&
    {
        return *metadata;
    }
} // namespace UKControllerPlugin::IntentionCode
