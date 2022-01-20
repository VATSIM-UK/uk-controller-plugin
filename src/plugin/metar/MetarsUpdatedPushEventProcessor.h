#pragma once
#include "push/PushEventProcessorInterface.h"

namespace UKControllerPlugin::Api {
    class ApiInterface;
} // namespace UKControllerPlugin::Api

namespace UKControllerPlugin::Metar {
    class MetarEventHandlerCollection;
    class ParsedMetarCollection;
    class ParsedMetarFactory;

    /**
     * Handles push events related to METARs updating
     */
    class MetarsUpdatedPushEventProcessor : public Push::PushEventProcessorInterface
    {
        public:
        MetarsUpdatedPushEventProcessor(
            ParsedMetarCollection& metars,
            const ParsedMetarFactory& factory,
            const Api::ApiInterface& api,
            const MetarEventHandlerCollection& eventHandlers);
        void ProcessPushEvent(const Push::PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const -> std::set<Push::PushEventSubscription> override;
        void PluginEventsSynced() override;

        private:
        void ProcessMetarsUpdatedJson(const nlohmann::json& data);

        // All the parsed metars
        ParsedMetarCollection& metars;

        // Builds the parsed metars from json
        const ParsedMetarFactory& factory;

        // The API for pulling all METAR updates
        const Api::ApiInterface& api;

        // Things that care about knowing when METARS update
        const MetarEventHandlerCollection& eventHandlers;
    };

} // namespace UKControllerPlugin::Metar
