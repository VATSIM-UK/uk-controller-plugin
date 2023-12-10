#include "MetarEventHandlerCollection.h"
#include "MetarEventHandlerInterface.h"

using UKControllerPlugin::Metar::MetarEventHandlerInterface;

namespace UKControllerPlugin::Metar {

    /*
        Returns the number of registered handlers.
    */
    auto MetarEventHandlerCollection::CountHandlers() const -> size_t
    {
        return this->handlers.size();
    }

    /*
        Loop through all the handlers and call their metar event.
    */
    void MetarEventHandlerCollection::UpdatedMetarEvent(const ParsedMetar& metar) const
    {
        for (const auto& handler : this->handlers) {
            try {
                handler->MetarUpdated(metar);
            } catch (const std::exception& e) {
                LogFatalExceptionAndRethrow(
                    "MetarEventHandlerCollection::UpdatedMetarEvent", typeid(handler).name(), e);
            }
        }
    }

    /*
        Add a handler to the collection.
    */
    void MetarEventHandlerCollection::RegisterHandler(std::shared_ptr<MetarEventHandlerInterface> handler)
    {
        if (!this->handlers.insert(handler).second) {
            LogWarning("Duplicate metar handler added");
        }
    }
} // namespace UKControllerPlugin::Metar
