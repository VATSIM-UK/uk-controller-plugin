#include "IntentionCodeCollection.h"
#include "IntentionCodeModel.h"

namespace UKControllerPlugin::IntentionCode {

    void IntentionCodeCollection::Add(std::shared_ptr<IntentionCodeModel> intentionCode)
    {
        if (intentionCodesById.contains(intentionCode->Id())) {
            LogWarning("Duplicate intention code added");
            return;
        }

        intentionCodes.push_back(intentionCode);
        intentionCodesById[intentionCode->Id()] = intentionCode;
    }

    auto IntentionCodeCollection::Count() const -> size_t
    {
        return intentionCodes.size();
    }

    auto IntentionCodeCollection::FindById(int id) const -> std::shared_ptr<IntentionCodeModel>
    {
        return intentionCodesById.contains(id) ? intentionCodesById.at(id) : nullptr;
    }

    auto IntentionCodeCollection::FirstWhere(const std::function<bool(const IntentionCodeModel&)> predicate) const
        -> std::shared_ptr<IntentionCodeModel>
    {
        for (const auto& code : intentionCodes) {
            if (predicate(*code)) {
                return code;
            }
        }

        return nullptr;
    }
} // namespace UKControllerPlugin::IntentionCode
