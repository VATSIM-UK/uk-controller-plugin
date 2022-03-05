#include "HoldingAircraft.h"
#include "HoldEventHandler.h"
#include "HoldManager.h"
#include "ProximityHold.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroscopeSectorFileElementInterface.h"
#include "navaids/NavaidCollection.h"
#include "tag/TagData.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPlugin::Hold {

    HoldEventHandler::HoldEventHandler(
        HoldManager& holdManager, const NavaidCollection& navaids, EuroscopePluginLoopbackInterface& plugin)
        : navaids(navaids), plugin(plugin), holdManager(holdManager)
    {
    }

    /*
        Return the description of the hold tag item
    */
    auto HoldEventHandler::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return "Selected Hold";
    }

    /*
        Return the value of the hold tag item
    */
    void HoldEventHandler::SetTagItemData(TagData& tagData)
    {
        std::shared_ptr<HoldingAircraft> aircraft =
            this->holdManager.GetHoldingAircraft(tagData.GetFlightplan().GetCallsign());
        if (!aircraft || aircraft->GetAssignedHold() == aircraft->GetNoHoldAssigned()) {
            tagData.SetItemString(this->noHold);
        } else {
            tagData.SetItemString("H" + aircraft->GetAssignedHold());
        }
    }

    void HoldEventHandler::TimedEventTrigger()
    {
        this->plugin.ApplyFunctionToAllFlightplans([this](
                                                       const std::shared_ptr<EuroScopeCFlightPlanInterface>& fp,
                                                       const std::shared_ptr<EuroScopeCRadarTargetInterface>& rt) {
            for (auto navaids = this->navaids.cbegin(); navaids != this->navaids.cend(); ++navaids) {
                if (rt->GetPosition().DistanceTo(navaids->coordinates) <= this->proximityDistance) {
                    this->holdManager.AddAircraftToProximityHold(
                        std::make_shared<ProximityHold>(fp->GetCallsign(), navaids->identifier));

                    if (rt->GetPosition().DistanceTo(navaids->coordinates) <= this->enterDistance) {
                        auto proximity = this->holdManager.GetHoldingAircraft(fp->GetCallsign())
                                             ->GetProximityHold(navaids->identifier);
                        if (!proximity->HasEntered()) {
                            proximity->Enter();
                        }
                    }
                } else {
                    this->holdManager.RemoveAircraftFromProximityHold(fp->GetCallsign(), navaids->identifier);
                }
            }
        });
    }

    void HoldEventHandler::ProcessPushEvent(const PushEvent& message)
    {
        if (message.event == "App\\Events\\HoldAssignedEvent") {
            if (message.data.is_object() && message.data.contains("callsign") &&
                message.data.at("callsign").is_string() && message.data.contains("navaid") &&
                message.data.at("navaid").is_string()) {
                this->holdManager.AssignAircraftToHold(
                    message.data.at("callsign").get<std::string>(),
                    message.data.at("navaid").get<std::string>(),
                    false);
            }
        } else if (message.event == "App\\Events\\HoldUnassignedEvent") {
            if (message.data.is_object() && message.data.contains("callsign") &&
                message.data.at("callsign").is_string()) {
                this->holdManager.UnassignAircraftFromHold(message.data.at("callsign").get<std::string>(), false);
            }
        }
    }

    auto HoldEventHandler::GetPushEventSubscriptions() const -> std::set<PushEventSubscription>
    {
        return {{PushEventSubscription::SUB_TYPE_CHANNEL, "private-hold-assignments"}};
    }
} // namespace UKControllerPlugin::Hold
