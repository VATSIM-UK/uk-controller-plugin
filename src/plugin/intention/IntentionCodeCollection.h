#pragma once

namespace UKControllerPlugin::IntentionCode {

    class IntentionCodeModel;

    /**
     * Collects all the intention codes.
     */
    class IntentionCodeCollection
    {
        public:
        void Add(std::shared_ptr<IntentionCodeModel> intentionCode);
        [[nodiscard]] auto Count() const -> size_t;
        [[nodiscard]] auto FindById(int id) const -> std::shared_ptr<IntentionCodeModel>;
        [[nodiscard]] auto FirstWhere(const std::function<bool(const IntentionCodeModel& model)> predicate) const
            -> std::shared_ptr<IntentionCodeModel>;

        private:
        // All the models
        std::list<std::shared_ptr<IntentionCodeModel>> intentionCodes;

        // All the models, but by id
        std::map<int, std::shared_ptr<IntentionCodeModel>> intentionCodesById;
    };

} // namespace UKControllerPlugin::IntentionCode
