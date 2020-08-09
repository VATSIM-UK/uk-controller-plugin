#include "pch/stdafx.h"
#include "hold/HoldEventHandler.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope//EuroScopeCRadarTargetInterface.h"
#include "hold/HoldManager.h"
#include "plugin/PopupMenuItem.h"
#include "euroscope/EuroscopeSectorFileElementInterface.h"
#include "websocket/WebsocketMessage.h"
#include "websocket/WebsocketSubscription.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Websocket::WebsocketMessage;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin {
    namespace Hold {

        HoldEventHandler::HoldEventHandler(
            HoldManager & holdManager,
            const NavaidCollection& navaids,
            EuroscopePluginLoopbackInterface & plugin,
            const int popupMenuItemId
        )
            : holdManager(holdManager), navaids(navaids), plugin(plugin),
            popupMenuItemId(popupMenuItemId)
        {

        }

        /*
            Return the description of the hold tag item
        */
        std::string HoldEventHandler::GetTagItemDescription(int tagItemId) const
        {
            return "Selected Hold";
        }

        /*
            Return the value of the hold tag item
        */
        void HoldEventHandler::SetTagItemData(TagData& tagData)
        {
            std::shared_ptr<HoldingAircraft> aircraft = this->holdManager.GetHoldingAircraft(
                tagData.flightPlan.GetCallsign()
            );
            if (!aircraft || aircraft->GetAssignedHold() == aircraft->noHoldAssigned) {
                tagData.SetItemString(this->noHold);
            } else {
                tagData.SetItemString("H" + aircraft->GetAssignedHold());
            }
        }

        void HoldEventHandler::TimedEventTrigger(void)
        {
            this->plugin.ApplyFunctionToAllFlightplans(
                [this](
                    std::shared_ptr<EuroScopeCFlightPlanInterface> fp,
                    std::shared_ptr<EuroScopeCRadarTargetInterface> rt
                ) {
                    for (
                        NavaidCollection::const_iterator navaids = this->navaids.cbegin();
                        navaids != this->navaids.cend();
                        ++navaids
                    ) {
                        if (rt->GetPosition().DistanceTo(navaids->coordinates) <= this->proximityDistance) {
                            this->holdManager.AddAircraftToProximityHold(fp->GetCallsign(), navaids->identifier);
                        } else {
                            this->holdManager.RemoveAircraftFromProximityHold(fp->GetCallsign(), navaids->identifier);
                        }
                    }

                }
            );
        }

        void HoldEventHandler::ProcessWebsocketMessage(const WebsocketMessage& message)
        {
            if (message.event == "App\\Events\\HoldAssignedEvent") {
                if (
                    message.data.is_object() &&
                    message.data.contains("callsign") &&
                    message.data.at("callsign").is_string() &&
                    message.data.contains("navaid") &&
                    message.data.at("navaid").is_string()
                ) {
                    this->holdManager.AssignAircraftToHold(
                        message.data.at("callsign").get<std::string>(),
                        message.data.at("navaid").get<std::string>(),
                        false
                    );
                }
            } else if (message.event == "App\\Events\\HoldUnassignedEvent") {
                if (
                    message.data.is_object() &&
                    message.data.contains("callsign") &&
                    message.data.at("callsign").is_string()
                ) {
                    this->holdManager.UnassignAircraftFromHold(message.data.at("callsign").get<std::string>(), false);
                }
            }
        }

        std::set<WebsocketSubscription> HoldEventHandler::GetSubscriptions(void) const
        {
            return {
                {
                    WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-hold-assignments"
                }
            };
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
