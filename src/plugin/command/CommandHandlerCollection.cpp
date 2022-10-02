#include "command/CommandHandlerCollection.h"
#include "command/CommandHandlerInterface.h"

using UKControllerPlugin::Command::CommandHandlerInterface;
namespace UKControllerPlugin {
    namespace Command {

        /*
            Returns the number of handlers in the collection.
        */
        size_t CommandHandlerCollection::CountHandlers(void) const
        {
            return this->handlers.size();
        }

        /*
            Registers the given handler with the collection.
        */
        void CommandHandlerCollection::RegisterHandler(std::shared_ptr<CommandHandlerInterface> handler)
        {
            this->handlers.push_back(handler);
        }

        /*
            Processes a given command by attempting to find a handler that will deal with it. Returns true
            if a handler sucessfully processes the command, false otherwise.
        */
        bool CommandHandlerCollection::ProcessCommand(std::string command) const
        {
            for (std::vector<std::shared_ptr<CommandHandlerInterface>>::const_iterator it = this->handlers.cbegin();
                 it != this->handlers.cend();
                 ++it) {
                if ((*it)->ProcessCommand(command)) {
                    return true;
                }
            }

            return false;
        }
    } // namespace Command
} // namespace UKControllerPlugin
