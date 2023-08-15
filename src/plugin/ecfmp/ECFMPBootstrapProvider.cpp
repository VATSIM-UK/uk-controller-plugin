#include "AircraftFlowMeasureMap.h"
#include "AircraftFlowMeasureTagItem.h"
#include "AircraftFlowMeasuresDialog.h"
#include "ECFMPBootstrapProvider.h"
#include "ECFMPModuleFactory.h"
#include "ListAircraftFlowMeasures.h"
#include "TriggerEcfmpEventLoop.h"
#include "bootstrap/ModuleFactories.h"
#include "bootstrap/PersistenceContainer.h"
#include "dialog/DialogManager.h"
#include "plugin/FunctionCallEventHandler.h"
#include "tag/TagFunction.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"

namespace UKControllerPlugin::ECFMP {

    void ECFMPBootstrapProvider::BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        // Event to trigger ECFMP event loop every second
        const auto ecfmpSdk = container.moduleFactories->ECFMP().Sdk(*container.curl);
        container.timedHandler->RegisterEvent(std::make_shared<TriggerECFMPEventLoop>(ecfmpSdk), 1);

        // Tag item to display flow measure for a given aircraft
        const auto aircraftFlowMeasureMap = std::make_shared<AircraftFlowMeasureMap>(*container.plugin);
        container.tagHandler->RegisterTagItem(
            131, std::make_shared<AircraftFlowMeasureTagItem>(aircraftFlowMeasureMap));

        // Create the dialog for displaying flow measures for an aircraft
        auto ecfmpFlowMeasuresDialog = std::make_shared<AircraftFlowMeasuresDialog>();
        container.dialogManager->AddDialog(
            {IDD_FLOW_MEASURE_LIST,
             "ECFMP Flow Measures",
             reinterpret_cast<DLGPROC>(ecfmpFlowMeasuresDialog->WndProc), // NOLINT
             reinterpret_cast<LPARAM>(ecfmpFlowMeasuresDialog.get()),     // NOLINT
             ecfmpFlowMeasuresDialog});

        // Create the tag function that triggers the flow measure dialog
        auto listAircraftFlowMeasuresTagFunctionHandler =
            std::make_shared<ListAircraftFlowMeasures>(aircraftFlowMeasureMap, *container.dialogManager);
        auto listAircraftFlowMeasuresTagFunction = Tag::TagFunction{
            9023,
            "ECFMP List Aircraft Flow Measures",
            [listAircraftFlowMeasuresTagFunctionHandler](const auto& flightplan, const auto&, auto, const auto&) {
                listAircraftFlowMeasuresTagFunctionHandler->ListForAircraft(flightplan);
            }};
        container.pluginFunctionHandlers->RegisterFunctionCall(listAircraftFlowMeasuresTagFunction);
    }
} // namespace UKControllerPlugin::ECFMP
