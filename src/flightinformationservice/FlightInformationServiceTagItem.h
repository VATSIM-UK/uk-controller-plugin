#pragma once
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin {
    namespace FlightInformationService {
        /*
            Handles the TAG item for Flight Information Services
        */
        class FlightInformationServiceTagItem : public Tag::TagItemInterface
        {
            public:
                std::string GetTagItemDescription(int tagItemId) const override;
                void SetTagItemData(Tag::TagData& tagData) override;

                const int flightInformationServiceTagItem = 116;
                const int flightInformationServiceOrBlankTagItem = 117;

            private:
        };
    }  // namespace FlightInformationService
}  // namespace UKControllerPlugin
