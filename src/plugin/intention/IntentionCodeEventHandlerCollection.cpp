#include "IntentionCodeEventHandlerCollection.h"
#include "IntentionCodeEventHandlerInterface.h"

namespace UKControllerPlugin::IntentionCode {

    void IntentionCodeEventHandlerCollection::AddHandler(std::shared_ptr<IntentionCodeEventHandlerInterface> handler)
    {
        if (!eventHandlers.insert(handler).second) {
            LogError("Duplicate intention code event handler added");
        }
    }

    auto IntentionCodeEventHandlerCollection::CountHandlers() const -> size_t
    {
        return eventHandlers.size();
    }

    void
    IntentionCodeEventHandlerCollection::IntentionCodeUpdated(const AircraftIntentionCode& aircraftIntentionCode) const
    {
        for (const auto& handler : eventHandlers) {
            try {
                handler->IntentionCodeUpdated(aircraftIntentionCode);
            } catch (const std::exception& e) {
                LogFatalExceptionAndRethrow(
                    "IntentionCodeEventHandlerCollection::IntentionCodeUpdated", typeid(handler).name(), e);
            }
        }
    }
} // namespace UKControllerPlugin::IntentionCode
