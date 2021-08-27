#include "FlightInformationServiceModule.h"
#include "FlightInformationServiceTagItem.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/CallbackFunction.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "tag/TagFunction.h"
#include "tag/TagItemCollection.h"

namespace UKControllerPlugin::FlightInformationService {
    const int openMenuTagFunctionId = 9009;

    void BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        int ukfisSelectedCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();

        std::shared_ptr<FlightInformationServiceTagItem> tagItem =
            std::make_shared<FlightInformationServiceTagItem>(*container.plugin, ukfisSelectedCallbackId);

        // TAG function to trigger the popup menu
        Tag::TagFunction openUkfisPopupMenu(
            openMenuTagFunctionId,
            "Open UK Flight Information Services Popup",
            [tagItem](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                std::string context,
                const POINT& mousePos) {
                tagItem->DisplayFlightInformationServicesMenu(fp, rt, std::move(context), mousePos);
            });

        container.pluginFunctionHandlers->RegisterFunctionCall(openUkfisPopupMenu);

        // The callback to select the UKFIS
        Euroscope::CallbackFunction ukfisSelectionCallback(
            ukfisSelectedCallbackId,
            "UKFIS Selected",
            [tagItem](int functionId, std::string subject, RECT screenObjectArea) {
                tagItem->MenuItemClicked(functionId, std::move(subject));
            });

        container.pluginFunctionHandlers->RegisterFunctionCall(ukfisSelectionCallback);

        container.tagHandler->RegisterTagItem(tagItem->flightInformationServiceTagItem, tagItem);
        container.tagHandler->RegisterTagItem(tagItem->flightInformationServiceOrBlankTagItem, tagItem);
        container.flightplanHandler->RegisterHandler(tagItem);
    }
} // namespace UKControllerPlugin::FlightInformationService
