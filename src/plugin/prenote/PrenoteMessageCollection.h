#pragma once
#include "ComparePrenoteMessages.h"

namespace UKControllerPlugin::Prenote {
    class PrenoteMessage;

    /**
     * Stores all the prenote messages within the plugin.
     */
    class PrenoteMessageCollection
    {
        public:
        void Add(const std::shared_ptr<PrenoteMessage>& prenote);
        [[nodiscard]] auto Count() const -> size_t;
        [[nodiscard]] auto GetById(int id) -> const std::shared_ptr<PrenoteMessage>&;
        void Remove(int id);

        using PrenoteMessages = std::set<std::shared_ptr<PrenoteMessage>, ComparePrenoteMessages>;
        using const_iterator = PrenoteMessages::const_iterator;
        [[nodiscard]] auto cbegin() const noexcept -> const_iterator;
        [[nodiscard]] auto cend() const noexcept -> const_iterator;

        private:
        const std::shared_ptr<PrenoteMessage> invalidMessage = nullptr;

        std::set<std::shared_ptr<PrenoteMessage>, ComparePrenoteMessages> prenotes;
    };
} // namespace UKControllerPlugin::Prenote
