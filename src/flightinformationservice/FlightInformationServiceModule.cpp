#include "pch/stdafx.h"
#include "flightinformationservice/FlightInformationServiceModule.h"
#include "flightinformationservice/FlightInformationServiceTagItem.h"
#include "bootstrap/PersistenceContainer.h"

namespace UKControllerPlugin {
    namespace FlightInformationService {
        void BootstrapPlugin(Bootstrap::PersistenceContainer& container)
        {
            std::shared_ptr<FlightInformationServiceTagItem> tagItem =
                std::make_shared<FlightInformationServiceTagItem>();

            container.tagHandler->RegisterTagItem(tagItem->flightInformationServiceTagItem, tagItem);
            container.tagHandler->RegisterTagItem(tagItem->flightInformationServiceOrBlankTagItem, tagItem);
        }
    }  // namespace FlightInformationService
}  // namespace UKControllerPlugin
