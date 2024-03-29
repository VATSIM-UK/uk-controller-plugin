#include "MetarEventHandlerCollection.h"
#include "MetarsUpdatedPushEventProcessor.h"
#include "ParsedMetarCollection.h"
#include "ParsedMetar.h"
#include "ParsedMetarFactory.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "push/PushEventSubscription.h"

namespace UKControllerPlugin::Metar {

    MetarsUpdatedPushEventProcessor::MetarsUpdatedPushEventProcessor(
        ParsedMetarCollection& metars,
        const ParsedMetarFactory& factory,
        const Api::ApiInterface& api,
        const MetarEventHandlerCollection& eventHandlers)
        : metars(metars), factory(factory), api(api), eventHandlers(eventHandlers)
    {
    }

    void MetarsUpdatedPushEventProcessor::ProcessPushEvent(const Push::PushEvent& message)
    {
        if (!message.data.is_object() || !message.data.contains("metars")) {
            LogError("Invalid METAR push event message");
            return;
        }

        this->ProcessMetarsUpdatedJson(message.data.at("metars"));
    }

    auto MetarsUpdatedPushEventProcessor::GetPushEventSubscriptions() const -> std::set<Push::PushEventSubscription>
    {
        return {{Push::PushEventSubscription::SUB_TYPE_EVENT, "metars.updated"}};
    }

    void MetarsUpdatedPushEventProcessor::PluginEventsSynced()
    {
        Async([this]() {
            try {
                this->ProcessMetarsUpdatedJson(this->api.GetAllMetars());
                LogInfo("Loaded " + std::to_string(this->metars.Count()) + " METARs");
            } catch (Api::ApiException) {
            }
        });
    }

    void MetarsUpdatedPushEventProcessor::ProcessMetarsUpdatedJson(const nlohmann::json& data)
    {
        if (!data.is_array()) {
            LogError("Invalid metar data: " + data.dump());
            return;
        }

        for (const auto& metarUpdate : data) {
            const auto parsed = factory.FromJson(metarUpdate);
            if (parsed == nullptr) {
                continue;
            }

            metars.UpdateMetar(parsed);
            eventHandlers.UpdatedMetarEvent(*parsed);
            LogInfo("Received updated METAR: " + parsed->Raw());
        }
    }
} // namespace UKControllerPlugin::Metar
