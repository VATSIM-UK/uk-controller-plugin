#pragma once

namespace UKControllerPlugin::IntentionCode {

    struct AircraftIntentionCode;
    class IntentionCodeEventHandlerInterface;

    /**
     * Contains classes that want to know about intention code events+
     */
    class IntentionCodeEventHandlerCollection
    {
        public:
        void AddHandler(std::shared_ptr<IntentionCodeEventHandlerInterface> handler);
        [[nodiscard]] auto CountHandlers() const -> size_t;
        void IntentionCodeUpdated(const AircraftIntentionCode& aircraftIntentionCode) const;

        private:
        // Things that want to handle the events
        std::set<std::shared_ptr<IntentionCodeEventHandlerInterface>> eventHandlers;
    };

} // namespace UKControllerPlugin::IntentionCode
