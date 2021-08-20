#include "prenote/PrenoteEventHandler.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "euroscope/GeneralSettingsEntries.h"
#include "euroscope/UserSetting.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "message/UserMessager.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "prenote/AbstractPrenote.h"
#include "prenote/PrenoteService.h"
#include "prenote/PrenoteUserMessage.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Prenote::PrenoteService;
using UKControllerPlugin::Prenote::PrenoteUserMessage;

namespace UKControllerPlugin::Prenote {

    PrenoteEventHandler::PrenoteEventHandler(std::unique_ptr<PrenoteService> prenoteService, UserSetting& userSetting)
        : prenoteService(std::move(prenoteService)), userSetting(userSetting)
    {
    }

    /*
        If the flightplan changes, cancel all sent prenotes.
    */
    void PrenoteEventHandler::FlightPlanEvent(
        EuroScopeCFlightPlanInterface& flightPlan, EuroScopeCRadarTargetInterface& radarTarget)
    {
        this->prenoteService->CancelPrenote(flightPlan);
    }

    /*
        If a flightplan disconnects, cancel all sent prenotes.
    */
    void PrenoteEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
    {
        this->prenoteService->CancelPrenote(flightPlan);
    }

    /*
        Handle events where the controller flightplan data has changed, in this case, when PUSH status is set.
    */
    void PrenoteEventHandler::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
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
    auto PrenoteEventHandler::PrenotesEnabled() const -> bool
    {
        return this->userSetting.GetBooleanEntry(GeneralSettingsEntries::usePrenoteSettingsKey, true);
    }
} // namespace UKControllerPlugin::Prenote
