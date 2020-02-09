#include "pch/stdafx.h"
#include "controller/ActiveCallsignMonitor.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ActiveCallsign.h"
#include "euroscope/EuroScopeCControllerInterface.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionParser.h"

using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionParser;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;

namespace UKControllerPlugin {
    namespace Controller {

        ActiveCallsignMonitor::ActiveCallsignMonitor(
            const ControllerPositionCollection & controllers,
            ActiveCallsignCollection & activeCallsigns
        )
            : activeCallsigns(activeCallsigns),
            controllers(controllers)
        {

        }

        /*
            Called when a controller disconnects from the network - remove from the active callsign list.
        */
        void ActiveCallsignMonitor::ControllerDisconnectEvent(EuroScopeCControllerInterface & controller)
        {
            // If we don't have the callsign, nothing to do.
            if (!this->activeCallsigns.CallsignActive(controller.GetCallsign())) {
                return;
            }

            ActiveCallsign active = this->activeCallsigns.GetCallsign(controller.GetCallsign());
            LogInfo(controller.GetCallsign() + " has disconnected or unset their primary frequency");
            this->activeCallsigns.RemoveCallsign(active);
        }

        /*
            Called when a controller update happens.
        */
        void ActiveCallsignMonitor::ControllerUpdateEvent(EuroScopeCControllerInterface & controller)
        {
            bool callsignActive = this->activeCallsigns.CallsignActive(controller.GetCallsign());
            bool frequencyActive = controller.HasActiveFrequency();

            // If it's already an active callsign with an active frequency, do nothing.
            if (callsignActive && frequencyActive) {
                return;
            }

            // If they're a registered callsign, but they've not got an active frequency, they must have unset primary.
            if (callsignActive && !frequencyActive) {
                this->ControllerDisconnectEvent(controller);
                return;
            }

            // They don't have an active frequency, which probably means they've just logged in.
            if (!frequencyActive) {
                return;
            }

            // Perform a match based on frequency and facility to find the canonical position
            ControllerPositionParser parser;
            try {
                this->SetupPosition(controller, this->controllers.FetchPositionByFacilityAndFrequency(
                    parser.ParseFacilityFromCallsign(controller.GetCallsign()),
                    controller.GetFrequency()
                ));
            }
            catch (std::out_of_range) {
                // No position at all, so nothing we can do.
                return;
            }
        }

        /*
            Called when the user is deemed to have logged out of Euroscope. Clear the entire online position
            cache.
        */
        void ActiveCallsignMonitor::SelfDisconnectEvent(void)
        {
            this->activeCallsigns.Flush();
        }

        /*
            Sets up a new controller position.
        */
        void ActiveCallsignMonitor::SetupPosition(
            EuroScopeCControllerInterface & callsign,
            const ControllerPosition & matchedPos
        ) {
            // Add to the active callsign collection.
            if (callsign.IsCurrentUser()) {
                LogInfo(
                    "The current user, with callsign " + callsign.GetCallsign() +
                    ", has been marked as active, covering " + matchedPos.GetCallsign()
                );
                this->activeCallsigns.AddUserCallsign(
                    ActiveCallsign(callsign.GetCallsign(), callsign.GetControllerName(), matchedPos)
                );
            } else {
                LogInfo(
                    callsign.GetCallsign() + " has been marked as active, covering " + matchedPos.GetCallsign()
                );
                this->activeCallsigns.AddCallsign(
                    ActiveCallsign(callsign.GetCallsign(), callsign.GetControllerName(), matchedPos)
                );
            }
        }
    }  // namespace Airfield
}   // namespace UKControllerPlugin
