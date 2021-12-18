#include "MetarsUpdatedPushEventProcessor.h"
#include "ParsedMetarCollection.h"
#include "ParsedMetarFactory.h"
#include "push/PushEventSubscription.h"

namespace UKControllerPlugin::Metar {

    MetarsUpdatedPushEventProcessor::MetarsUpdatedPushEventProcessor(
        ParsedMetarCollection& metars, const ParsedMetarFactory& factory)
        : metars(metars), factory(factory)
    {
    }

    void MetarsUpdatedPushEventProcessor::ProcessPushEvent(const Push::PushEvent& message)
    {
        const nlohmann::json& data = message.data;
        if (!data.is_array()) {
            LogError("Invalid metar updated message: " + data.dump());
            return;
        }

        for (const auto& metarUpdate : data) {
            const auto parsed = factory.FromJson(metarUpdate);
            if (parsed == nullptr) {
                continue;
            }

            metars.UpdateMetar(parsed);
        }
    }

    auto MetarsUpdatedPushEventProcessor::GetPushEventSubscriptions() const -> std::set<Push::PushEventSubscription>
    {
        return {{Push::PushEventSubscription::SUB_TYPE_EVENT, "metars.updated"}};
    }
} // namespace UKControllerPlugin::Metar
