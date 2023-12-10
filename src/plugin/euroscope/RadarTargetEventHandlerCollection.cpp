#include "euroscope/RadarTargetEventHandlerCollection.h"
#include "euroscope/RadarTargetEventHandlerInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::RadarTargetEventHandlerInterface;

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            Returns the number of registered handlers.
        */
        int RadarTargetEventHandlerCollection::CountHandlers(void) const
        {
            return this->handlerList.size();
        }

        /*
            Called whenever there's a radar target position update.
        */
        void RadarTargetEventHandlerCollection::RadarTargetEvent(EuroScopeCRadarTargetInterface& radarTarget) const
        {
            // Loop through the handlers and call their handling function.
            for (std::set<std::shared_ptr<RadarTargetEventHandlerInterface>>::const_iterator it =
                     this->handlerList.cbegin();
                 it != this->handlerList.cend();
                 ++it) {
                try {
                    (*it)->RadarTargetPositionUpdateEvent(radarTarget);
                } catch (const std::exception& exception) {
                    LogFatalExceptionAndRethrow(
                        "RadarTargetEventHandlerCollection::RadarTargetEvent",
                        std::string(typeid(*it).name()),
                        exception);
                }
            }
        }

        /*
            Registers an object to handle and event.
        */
        void
        RadarTargetEventHandlerCollection::RegisterHandler(std::shared_ptr<RadarTargetEventHandlerInterface> handler)
        {
            this->handlerList.insert(handler);
        }
    } // namespace Euroscope
} // namespace UKControllerPlugin
