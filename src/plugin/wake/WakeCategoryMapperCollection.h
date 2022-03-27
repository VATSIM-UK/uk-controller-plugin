#pragma once

namespace UKControllerPlugin::Wake {
    class WakeCategoryMapperInterface;

    /**
     * Contains a bunch of wake category mappers
     */
    class WakeCategoryMapperCollection
    {
        public:
        void Add(int wakeSchemeId, std::shared_ptr<WakeCategoryMapperInterface> mapper);
        auto Get(int wakeSchemeId) const -> std::shared_ptr<WakeCategoryMapperInterface>;
        [[nodiscard]] auto Count() const -> size_t;

        private:
        // All the mappers
        std::map<int, std::shared_ptr<WakeCategoryMapperInterface>> mappers;
    };
} // namespace UKControllerPlugin::Wake
