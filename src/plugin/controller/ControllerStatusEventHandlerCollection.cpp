#include "controller/ControllerStatusEventHandlerCollection.h"
#include "controller/ControllerStatusEventHandlerInterface.h"
#include "euroscope/EuroScopeCControllerInterface.h"

using UKControllerPlugin::Controller::ControllerStatusEventHandlerInterface;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;

namespace UKControllerPlugin {
    namespace Controller {

        /*
            Called when a "Controller Update" event happens - logging on position changing.
        */
        void
        ControllerStatusEventHandlerCollection::ControllerUpdateEvent(EuroScopeCControllerInterface& controller) const
        {
            for (std::vector<std::shared_ptr<ControllerStatusEventHandlerInterface>>::const_iterator it =
                     this->eventHandlers.begin();
                 it != this->eventHandlers.end();
                 ++it) {
                try {
                    (*it)->ControllerUpdateEvent(controller);
                } catch (const std::exception& exception) {
                    LogFatalExceptionAndRethrow(
                        "ControllerStatusEventHandlerCollection::ControllerUpdateEvent",
                        std::string(typeid(*it).name()),
                        exception);
                }
            }
        }

        /*
            Called when a "Controller Disconnect" happens.
        */
        void ControllerStatusEventHandlerCollection::ControllerDisconnectEvent(
            EuroScopeCControllerInterface& controller) const
        {
            for (std::vector<std::shared_ptr<ControllerStatusEventHandlerInterface>>::const_iterator it =
                     this->eventHandlers.begin();
                 it != this->eventHandlers.end();
                 ++it) {
                try {
                    (*it)->ControllerDisconnectEvent(controller);
                } catch (const std::exception& exception) {
                    LogFatalExceptionAndRethrow(
                        "ControllerStatusEventHandlerCollection::ControllerDisconnectEvent",
                        std::string(typeid(*it).name()),
                        exception);
                }
            }
        }

        int ControllerStatusEventHandlerCollection::CountHandlers(void) const
        {
            return this->eventHandlers.size();
        }

        /*
            Adds an event handler to the collection.
        */
        void ControllerStatusEventHandlerCollection::RegisterHandler(
            std::shared_ptr<ControllerStatusEventHandlerInterface> handler)
        {
            this->eventHandlers.push_back(handler);
        }

        /*
            Called when the current user disconnects as a controller.
        */
        void ControllerStatusEventHandlerCollection::SelfDisconnectEvent(void) const
        {
            for (std::vector<std::shared_ptr<ControllerStatusEventHandlerInterface>>::const_iterator it =
                     this->eventHandlers.begin();
                 it != this->eventHandlers.end();
                 ++it) {
                try {
                    (*it)->SelfDisconnectEvent();
                } catch (const std::exception& exception) {
                    LogFatalExceptionAndRethrow(
                        "ControllerStatusEventHandlerCollection::SelfDisconnectEvent",
                        std::string(typeid(*it).name()),
                        exception);
                }
            }
        }
    } // namespace Controller
} // namespace UKControllerPlugin
