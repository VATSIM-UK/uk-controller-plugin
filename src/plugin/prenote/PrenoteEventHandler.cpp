#include "pch/pch.h"
#include "prenote/PrenoteEventHandler.h"
#include "prenote/AbstractPrenote.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "message/UserMessager.h"
#include "prenote/PrenoteUserMessage.h"
#include "euroscope/UserSetting.h"
#include "prenote/PrenoteService.h"
#include "euroscope/GeneralSettingsEntries.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Prenote::PrenoteUserMessage;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Prenote::PrenoteService;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;

namespace UKControllerPlugin {
    namespace Prenote {

        PrenoteEventHandler::PrenoteEventHandler(
            std::unique_ptr<PrenoteService> prenoteService,
            UserSetting & userSetting
        )
            : prenoteService(std::move(prenoteService)), userSetting(userSetting)
        {

        }

        /*
            If the flightplan changes, cancel all sent prenotes.
        */
        void PrenoteEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            this->prenoteService->CancelPrenote(flightPlan);
        }

        /*
            If a flightplan disconnects, cancel all sent prenotes.
        */
        void PrenoteEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface & flightPlan)
        {
            this->prenoteService->CancelPrenote(flightPlan);
        }

        /*
            Handle events where the controller flightplan data has changed, in this case, when PUSH status is set.
        */
        void PrenoteEventHandler::ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            int dataType
        ) {
            if (!this->PrenotesEnabled()) {
                return;
            }

            if (dataType != EuroScopePlugIn::CTR_DATA_TYPE_GROUND_STATE) {
                return;
            }

            if (flightPlan.GetGroundState() != "PUSH") {
                return;
            }

            this->prenoteService->SendPrenotes(flightPlan);
        }

        /*
            Returns true if prenotes are enabled.
        */
        bool PrenoteEventHandler::PrenotesEnabled(void) const
        {
            return this->userSetting.GetBooleanEntry(GeneralSettingsEntries::usePrenoteSettingsKey, true);
        }
    }  // namespace Prenote
}  // namespace UKControllerPlugin
