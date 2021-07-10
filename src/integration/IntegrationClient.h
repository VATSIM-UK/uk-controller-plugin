#pragma once

namespace UKControllerPlugin::Integration {
    class IntegrationConnection;
    struct MessageType;
    class MessageInterface;

    /*
     * Stores information about an integrating application - what types of events they
     * are interested in etc.
     */
    class IntegrationClient
    {
        public:
            const std::set<const std::shared_ptr<MessageType>>& InterestedMessages() const;
            bool InterestedInMessage(const MessageType& message) const;
            void ClearInterestedMessages();
            void AddInterestedMessage(std::shared_ptr<MessageType> message);
            void Send(std::shared_ptr<MessageInterface> message) const;
            std::shared_ptr<MessageInterface> Receive();
            bool Active() const;

        private:
            std::string GetIntegrationString() const;
            static bool MessageFormatValid(const nlohmann::json& message);

            // Name of the integration
            std::string integrationName;

            // Version of the integration
            std::string integrationVersion;

            // The messages that this target is interested in receiving
            std::set<const std::shared_ptr<MessageType>> interestedMessages;

            // The connection for this client
            const std::shared_ptr<IntegrationConnection> connection;
    };
} // namespace UKControllerPlugin::Integration
