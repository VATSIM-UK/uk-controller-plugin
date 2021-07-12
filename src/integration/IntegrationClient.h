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
            IntegrationClient(
                std::string integrationName,
                std::string integrationVersion,
                std::shared_ptr<IntegrationConnection> connection
            );
            ~IntegrationClient();
            const std::set<const std::shared_ptr<MessageType>>& InterestedMessages() const;
            bool InterestedInMessage(const MessageType& message) const;
            void ClearInterestedMessages();
            void AddInterestedMessage(std::shared_ptr<MessageType> message);
            const std::shared_ptr<IntegrationConnection> Connection() const;

        private:
            std::string GetIntegrationString() const;

            // Name of the integration
            std::string integrationName = "Unknown";

            // Version of the integration
            std::string integrationVersion = "Unknown";

            // The messages that this target is interested in receiving
            std::set<const std::shared_ptr<MessageType>> interestedMessages;

            // The connection for this client
            const std::shared_ptr<IntegrationConnection> connection;
    };
} // namespace UKControllerPlugin::Integration
