#include "ownership/AirfieldOwnerQueryMessage.h"

namespace UKControllerPlugin {
    namespace Ownership {

        AirfieldOwnerQueryMessage::AirfieldOwnerQueryMessage(
            std::string airfield, std::string callsign, std::string name)
            : airfield(airfield), callsign(callsign), name(name)
        {
        }

        /*
         *   Put the message in a dedicated handler
         */
        std::string AirfieldOwnerQueryMessage::MessageHandler(void) const
        {
            return "UKCP_Query";
        }

        /*
         *   The message sender should be the plugin
         */
        std::string AirfieldOwnerQueryMessage::MessageSender(void) const
        {
            return "UKCP";
        }

        /*
         *   Format the message into something for the user.
         */
        std::string AirfieldOwnerQueryMessage::MessageString(void) const
        {
            if (this->callsign == "") {
                return "Airfield " + this->airfield + " is not owned by any controller";
            }

            return "Airfield " + this->airfield + " is owned by " + this->callsign + " (" + this->name + ")";
        }

        /*
         *   The handler should be shown
         */
        bool AirfieldOwnerQueryMessage::MessageShowHandler(void) const
        {
            return true;
        }

        /*
         *   The message handler should be marked as unread
         */
        bool AirfieldOwnerQueryMessage::MessageMarkUnread(void) const
        {
            return true;
        }

        /*
         *   If they've typed this command they've asked for it, so override busy
         */
        bool AirfieldOwnerQueryMessage::MessageOverrideBusy(void) const
        {
            return true;
        }

        /*
         *  Make it easy to see the handler
         */
        bool AirfieldOwnerQueryMessage::MessageFlashHandler(void) const
        {
            return true;
        }

        /*
         *   Don't make them click too much
         */
        bool AirfieldOwnerQueryMessage::MessageRequiresConfirm(void) const
        {
            return false;
        }

    } // namespace Ownership
} // namespace UKControllerPlugin
