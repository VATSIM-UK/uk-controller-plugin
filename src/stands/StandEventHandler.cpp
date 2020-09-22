#include "pch/stdafx.h"
#include "stands/StandEventHandler.h"

namespace UKControllerPlugin {
    namespace Stands {

        StandEventHandler::StandEventHandler(const std::set<Stand, CompareStands> stands)
            : stands(stands)
        {
        }

        size_t StandEventHandler::CountStands(void) const
        {
            return this->stands.size();
        }

        size_t StandEventHandler::CountStandAssignments(void) const
        {
            return this->standAssignments.size();
        }

        void StandEventHandler::SetAssignedStand(std::string callsign, int standId)
        {
            this->standAssignments[callsign] = standId;
        }

        std::string StandEventHandler::GetTagItemDescription(int tagItemId) const
        {
            return "Assigned Stand";
        }

        void StandEventHandler::SetTagItemData(UKControllerPlugin::Tag::TagData& tagData)
        {
            if (
                this->standAssignments.count(tagData.flightPlan.GetCallsign())
            ) {
                auto stand = this->stands.find(this->standAssignments.at(tagData.flightPlan.GetCallsign()));
                if (stand == this->stands.cend()) {
                    return;
                }

                tagData.SetItemString(stand->identifier);
            }
        }
    }  // namespace Stands
}  // namespace UKControllerPlugin
