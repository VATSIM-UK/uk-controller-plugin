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
        void Iterate(std::function<void(const std::shared_ptr<PrenoteMessage>&)>& #function);
        void Remove(int id);

        private:
        [[nodiscard]] auto GetByIdUnsafe(int id) -> const std::shared_ptr<PrenoteMessage>&;
        [[nodiscard]] auto LockPrenotes() -> std::lock_guard<std::mutex>;

        // The message to return if somsone asks for one that doesn't exist
        const std::shared_ptr<PrenoteMessage> invalidMessage = nullptr;

        // Stores all the prenotes sorted by id
        std::set<std::shared_ptr<PrenoteMessage>, ComparePrenoteMessages> prenotes;

        std::mutex prenoteLock;
    };
} // namespace UKControllerPlugin::Prenote
