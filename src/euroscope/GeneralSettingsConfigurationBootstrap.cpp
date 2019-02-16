#include "pch/stdafx.h"
#include "euroscope/GeneralSettingsConfigurationBootstrap.h"
#include "euroscope/GeneralSettingsConfiguration.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "windows/WinApiInterface.h"
#include "euroscope/CallbackFunction.h"
#include "command/CommandHandlerCollection.h"

using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Command::CommandHandlerCollection;

namespace UKControllerPlugin {
    namespace Euroscope {

        void GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
            FunctionCallEventHandler & functionHandler,
            ConfigurableDisplayCollection & configurableDisplays,
            CommandHandlerCollection & commandHandlers,
            WinApiInterface & winApi
        ) {
            int callbackFunctionId = functionHandler.ReserveNextDynamicFunctionId();
            std::shared_ptr<GeneralSettingsConfiguration> dialog = std::make_shared<GeneralSettingsConfiguration>(
                winApi,
                callbackFunctionId
            );
            CallbackFunction showDialogFunction(
                callbackFunctionId,
                "Open General Settings Dialog",
                std::bind(
                    &GeneralSettingsConfiguration::Configure,
                    dialog,
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            );

            functionHandler.RegisterFunctionCall(showDialogFunction);
            configurableDisplays.RegisterDisplay(dialog);
            commandHandlers.RegisterHandler(dialog);
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
