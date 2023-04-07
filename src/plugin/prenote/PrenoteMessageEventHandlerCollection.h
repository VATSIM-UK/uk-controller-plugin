#pragma once

namespace UKControllerPlugin::Prenote {
    class PrenoteMessage;
    class PrenoteMessageEventHandlerInterface;

    /**
     * All the prenote message event handlers.
     */
    class PrenoteMessageEventHandlerCollection
    {
        public:
        void AddHandler(const std::shared_ptr<PrenoteMessageEventHandlerInterface>& handler);
        [[nodiscard]] auto CountHandlers() const -> size_t;
        void MessageAcknowledged(const PrenoteMessage& message) const;
        void MessageCancelled(const PrenoteMessage& message) const;
        void NewMessage(const PrenoteMessage& message) const;
        void MessageTimeout(const PrenoteMessage& message) const;

        private:
        // All the handlers
        std::set<std::shared_ptr<PrenoteMessageEventHandlerInterface>> handlers;
    };
} // namespace UKControllerPlugin::Prenote
