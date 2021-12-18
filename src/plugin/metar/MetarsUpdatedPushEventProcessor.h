#pragma once
#include "push/PushEventProcessorInterface.h"

namespace UKControllerPlugin::Metar {
    class ParsedMetarCollection;
    class ParsedMetarFactory;

    /**
     * Handles push events related to METARs updating
     */
    class MetarsUpdatedPushEventProcessor : public Push::PushEventProcessorInterface
    {
        public:
        MetarsUpdatedPushEventProcessor(ParsedMetarCollection& metars, const ParsedMetarFactory& factory);
        void ProcessPushEvent(const Push::PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const -> std::set<Push::PushEventSubscription> override;

        private:
        // All the parsed metars
        ParsedMetarCollection& metars;

        // Builds the parsed metars from json
        const ParsedMetarFactory& factory;
    };

} // namespace UKControllerPlugin::Metar
