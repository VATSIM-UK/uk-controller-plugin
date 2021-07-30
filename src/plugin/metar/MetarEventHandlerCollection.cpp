#include "pch/pch.h"
#include "metar/MetarEventHandlerCollection.h"
#include "metar/MetarEventHandlerInterface.h"

using UKControllerPlugin::Metar::MetarEventHandlerInterface;

namespace UKControllerPlugin {
    namespace Metar {


        /*
            Returns the number of registered handlers.
        */
        int MetarEventHandlerCollection::CountHandlers(void) const
        {
            return this->handlers.size();
        }

        /*
            Loop through all the handlers and call their metar event.
        */
        void MetarEventHandlerCollection::NewMetarEvent(std::string station, std::string metar) const
        {
            for (
                std::set<std::shared_ptr<MetarEventHandlerInterface>>::const_iterator it = this->handlers.cbegin();
                it != this->handlers.cend();
                ++it
            ) {
                (*it)->NewMetar(station, metar);
            }
        }

        /*
            Add a handler to the collection.
        */
        void MetarEventHandlerCollection::RegisterHandler(std::shared_ptr<MetarEventHandlerInterface> handler)
        {
            this->handlers.insert(handler);
        }
    }  // namespace Metar
}  // namespace UKControllerPlugin
