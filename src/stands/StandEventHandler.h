#pragma once
#include "stands/Stand.h"
#include "stands/CompareStands.h"
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin {
    namespace Stands {
        /*
            A new class
        */
        class StandEventHandler: public UKControllerPlugin::Tag::TagItemInterface
        {
            public:
                StandEventHandler(
                    std::set<UKControllerPlugin::Stands::Stand, UKControllerPlugin::Stands::CompareStands> stands
                );

                size_t CountStands(void) const;
                size_t CountStandAssignments(void) const;
                void SetAssignedStand(std::string callsign, int standId);

                // Inherited via TagItemInterface
                std::string GetTagItemDescription(int tagItemId) const override;
                void SetTagItemData(UKControllerPlugin::Tag::TagData& tagData) override;

            private:

                // All the stands we have
                std::set<UKControllerPlugin::Stands::Stand, UKControllerPlugin::Stands::CompareStands> stands;

                // The currently assigned stands and who they are assigned to
                std::map<std::string, int> standAssignments;
        };
    }  // namespace Stands
}  // namespace UKControllerPlugin
