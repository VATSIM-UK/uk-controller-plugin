#include "IntegrationClient.h"
#include "IntegrationDataInitialiser.h"
#include "IntegrationDataInitialisers.h"
#include "MessageType.h"

namespace UKControllerPlugin::Integration {

    void IntegrationDataInitialisers::Add(const std::shared_ptr<IntegrationDataInitialiser>& initialiser)
    {
        if (HasInitialiser(initialiser)) {
            LogWarning("Duplicate integration initialiser added");
            return;
        }

        initialisers.push_back(initialiser);
    }

    auto IntegrationDataInitialisers::Count() const -> size_t
    {
        return initialisers.size();
    }

    void IntegrationDataInitialisers::InitialiseClient(const IntegrationClient& client) const
    {
        for (const auto& initialiser : initialisers) {
            if (!client.InterestedInMessage(initialiser->InitialisesFor())) {
                continue;
            }

            initialiser->Initialise(client);
        }
    }

    auto IntegrationDataInitialisers::HasInitialiser(std::shared_ptr<IntegrationDataInitialiser> initialiser) const
        -> bool
    {
        return std::find(initialisers.begin(), initialisers.end(), initialiser) != initialisers.cend();
    }
} // namespace UKControllerPlugin::Integration
