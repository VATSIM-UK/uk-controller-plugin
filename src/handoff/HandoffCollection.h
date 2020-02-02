#pragma once
#include "controller/ControllerPositionHierarchy.h"

namespace UKControllerPlugin {
    namespace Handoff {

        /*
            A class that stores all the handoff orders available from the plugin
            and how each SID maps to a handoff order.
        */
        class HandoffCollection
        {
            public:

                void AddHandoffOrder(
                    std::string key,
                    std::shared_ptr<UKControllerPlugin::Controller::ControllerPositionHierarchy> controllers
                );
                void AddSidMapping(std::string airfield, std::string identifier, std::string handoffKey);
                const UKControllerPlugin::Controller::ControllerPositionHierarchy& GetSidHandoffOrder(
                    std::string airfield,
                    std::string identifier
                ) const;
                size_t CountSidMappings(void) const;
                size_t CountHandoffs(void) const;

                const UKControllerPlugin::Controller::ControllerPositionHierarchy invalidHierarchy;

            private:
                
                std::string GetStorageKeyForSid(std::string airfield, std::string identifier) const;

                // All the handoff orders
                std::map<std::string, std::shared_ptr<UKControllerPlugin::Controller::ControllerPositionHierarchy>> orders;

                // The SID -> handoff mappings
                std::map<std::string, std::string> sidMappings;
        };
    }  // namespace UKControllerPlugin
}  // namespacem Handoff
