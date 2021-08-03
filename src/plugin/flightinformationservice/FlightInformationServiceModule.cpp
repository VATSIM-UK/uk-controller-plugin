#include "pch/pch.h"
#include "flightinformationservice/FlightInformationServiceModule.h"
#include "flightinformationservice/FlightInformationServiceTagItem.h"
#include "bootstrap/PersistenceContainer.h"
#include "tag/TagFunction.h"
#include "euroscope/CallbackFunction.h"

namespace UKControllerPlugin {
    namespace FlightInformationService {
        const int openMenuTagFunctionId = 9009;

        void BootstrapPlugin(Bootstrap::PersistenceContainer& container)
        {
            int ukfisSelectedCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();

            std::shared_ptr<FlightInformationServiceTagItem> tagItem =
                std::make_shared<FlightInformationServiceTagItem>(
                    *container.plugin,
                    ukfisSelectedCallbackId
                );


            // TAG function to trigger the popup menu
            Tag::TagFunction openUkfisPopupMenu(
                openMenuTagFunctionId,
                "Open UK Flight Information Services Popup",
                std::bind(
                    &FlightInformationServiceTagItem::DisplayFlightInformationServicesMenu,
                    tagItem,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openUkfisPopupMenu);

            // The callback to select the UKFIS
            Euroscope::CallbackFunction ukfisSelectionCallback(
                ukfisSelectedCallbackId,
                "UKFIS Selected",
                std::bind(
                    &FlightInformationServiceTagItem::MenuItemClicked,
                    tagItem,
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(ukfisSelectionCallback);

            container.tagHandler->RegisterTagItem(tagItem->flightInformationServiceTagItem, tagItem);
            container.tagHandler->RegisterTagItem(tagItem->flightInformationServiceOrBlankTagItem, tagItem);
            container.flightplanHandler->RegisterHandler(tagItem);
        }
    }  // namespace FlightInformationService
}  // namespace UKControllerPlugin
