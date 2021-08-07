#pragma once
#include "integration/MessageInterface.h"

namespace UKControllerPlugin::Stands {
    class StandAssignedMessage: public Integration::MessageInterface
    {
        public:
            StandAssignedMessage(std::string callsign, std::string airfield, std::string stand);
            ~StandAssignedMessage() override = default;
            nlohmann::json GetMessageData() const override;
            Integration::MessageType GetMessageType() const override;
            
        private:
            // The callsign for the assignment
            std::string callsign;
            
            // The airfield the stand is at
            std::string airfield;
            
            // The stand identifier
            std::string stand;
    };
} // namespace UKControllerPlugin::Stands
