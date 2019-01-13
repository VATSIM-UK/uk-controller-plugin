#include "pch/stdafx.h"
#include "controller/AirfieldsOwnedQueryMessage.h"

namespace UKControllerPlugin {
    namespace Controller {

        AirfieldsOwnedQueryMessage::AirfieldsOwnedQueryMessage(
            std::vector<UKControllerPlugin::Airfield::Airfield> airfields,
            std::string userCallsign
        )
            : airfields(airfields), userCallsign(userCallsign)
        {

        }

        /*
        *   Put the message in a dedicated handler
        */
        std::string AirfieldsOwnedQueryMessage::MessageHandler(void) const
        {
            return "UKCP_Query";
        }

        /*
        *   The message sender should be the plugin
        */
        std::string AirfieldsOwnedQueryMessage::MessageSender(void) const
        {
            return "UKCP";
        }

        /*
        *   Format the message into something for the user.
        */
        std::string AirfieldsOwnedQueryMessage::MessageString(void) const
        {
            if (this->airfields.size() == 0) {
                return this->userCallsign + " does not own any airfields";
            }

            std::string airfieldString;
            for (
                std::vector<UKControllerPlugin::Airfield::Airfield>::const_iterator it = this->airfields.cbegin();
                it != this->airfields.cend();
                ++it
            ) {
                airfieldString = airfieldString + it->GetIcao() + ", ";
            }

            // Take the comma and space off the airfield string
            airfieldString = airfieldString.substr(0, airfieldString.size() - 2);

            return this->userCallsign + " owns the following airfields: " + airfieldString;
        }

        /*
        *   The handler should be shown
        */
        bool AirfieldsOwnedQueryMessage::MessageShowHandler(void) const
        {
            return true;
        }

        /*
        *   The message handler should be marked as unread
        */
        bool AirfieldsOwnedQueryMessage::MessageMarkUnread(void) const
        {
            return true;
        }

        /*
        *   If they've typed this command they've asked for it, so override busy
        */
        bool AirfieldsOwnedQueryMessage::MessageOverrideBusy(void) const
        {
            return true;
        }

        /*
        *  Make it easy to see the handler
        */
        bool AirfieldsOwnedQueryMessage::MessageFlashHandler(void) const
        {
            return true;
        }

        /*
        *   Don't make them click too much
        */
        bool AirfieldsOwnedQueryMessage::MessageRequiresConfirm(void) const
        {
            return false;
        }

    }  // namespace Controller
}  // namespace UKControllerPlugin
