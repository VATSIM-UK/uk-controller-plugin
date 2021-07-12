#include "pch/stdafx.h"
#include "integration/ClientInitialisationManager.h"
#include "integration/IntegrationConnection.h"
#include "time/SystemClock.h"

namespace UKControllerPlugin::Integration {

    ClientInitialisationManager::ClientInitialisationManager(std::shared_ptr<IntegrationClientManager> clientManager):
        clientManager(std::move(clientManager)) {}

    void ClientInitialisationManager::AddConnection(std::shared_ptr<IntegrationConnection> connection)
    {
        if (this->connections.count(connection)) {
            LogWarning("A duplicate integration client was added");
            return;
        }

        this->connections[connection] = Time::TimeNow();
    }

    void ClientInitialisationManager::TimedEventTrigger()
    {
        for (
            auto connection = this->connections.begin();
            connection != this->connections.end();
        ) {
            // If it's taken too long to initialise, then kill the connection
            if (Time::TimeNow() < connection->second + std::chrono::seconds(10)) {
                LogInfo("Integration has not initialised in time");
                this->connections.erase(connection++);
            }

            // Check for an init message and if so, elevate to client level, ignore anything else
            std::string nextMessage;
            while ((nextMessage = connection->first->Receive()) != connection->first->NO_MESSAGES_PENDING) { }

            ++connection;
        }
    }


} // namespace UKControllerPlugin::Integration
