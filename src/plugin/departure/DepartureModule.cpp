#include "DepartureModule.h"
#include "DepartureCoordinationList.h"
#include "ToggleDepartureCoordinationList.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/CallbackFunction.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/RadarRenderableCollection.h"

using UKControllerPlugin::Euroscope::CallbackFunction;

namespace UKControllerPlugin::Departure {
    void BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& container,
        RadarScreen::RadarRenderableCollection& renderables,
        RadarScreen::ConfigurableDisplayCollection& configurables,
        Euroscope::AsrEventHandlerCollection& asrHandlers)
    {
        // Coordination list
        const int coordinationListRendererId = renderables.ReserveRendererIdentifier();
        auto coordinationList = std::make_shared<DepartureCoordinationList>(
            *container.departureReleaseHandler,
            *container.plugin,
            *container.controllerPositions,
            renderables.ReserveScreenObjectIdentifier(coordinationListRendererId));

        renderables.RegisterRenderer(coordinationListRendererId, coordinationList, renderables.afterLists);
        asrHandlers.RegisterHandler(coordinationList);

        // Create the configuration list item
        const int requestListShowCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        auto listItem = std::make_shared<ToggleDepartureCoordinationList>(coordinationList, requestListShowCallbackId);

        CallbackFunction showReleaseRequestListCallback(
            requestListShowCallbackId,
            "Toggle Departure Coordination List",
            [listItem](int functionId, std::string subject, RECT screenObjectArea) {
                listItem->Configure(functionId, std::move(subject), screenObjectArea);
            });

        container.pluginFunctionHandlers->RegisterFunctionCall(showReleaseRequestListCallback);
        configurables.RegisterDisplay(listItem);
    }
} // namespace UKControllerPlugin::Departure
