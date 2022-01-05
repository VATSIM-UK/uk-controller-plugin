#include "euroscope/GeneralSettingsConfigurationBootstrap.h"
#include "euroscope/GeneralSettingsConfiguration.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "dialog/DialogManager.h"
#include "dialog/DialogData.h"
#include "euroscope/CallbackFunction.h"
#include "command/CommandHandlerCollection.h"
#include "euroscope/GeneralSettingsDialog.h"
#include "euroscope/UserSettingAwareCollection.h"
#include "setting/SettingRepository.h"

using UKControllerPlugin::Command::CommandHandlerCollection;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Euroscope::GeneralSettingsDialog;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;

namespace UKControllerPlugin {
    namespace Euroscope {

        void GeneralSettingsConfigurationBootstrap::BootstrapPlugin(
            DialogManager& dialogManager,
            UserSetting& userSettings,
            UserSettingAwareCollection& userSettingsHandlers,
            Setting::SettingRepository& settings)
        {
            std::shared_ptr<GeneralSettingsDialog> dialog =
                std::make_shared<GeneralSettingsDialog>(userSettings, userSettingsHandlers, settings);
            dialogManager.AddDialog(
                {IDD_GENERAL_SETTINGS,
                 "General Settings",
                 reinterpret_cast<DLGPROC>(dialog->WndProc),
                 reinterpret_cast<LPARAM>(dialog.get()),
                 dialog});
        }

        void GeneralSettingsConfigurationBootstrap::BootstrapRadarScreen(
            FunctionCallEventHandler& functionHandler,
            ConfigurableDisplayCollection& configurableDisplays,
            CommandHandlerCollection& commandHandlers,
            const DialogManager& dialogManager)
        {
            int callbackFunctionId = functionHandler.ReserveNextDynamicFunctionId();
            std::shared_ptr<GeneralSettingsConfiguration> dialog =
                std::make_shared<GeneralSettingsConfiguration>(dialogManager, callbackFunctionId);
            CallbackFunction showDialogFunction(
                callbackFunctionId,
                "Open General Settings Dialog",
                std::bind(
                    &GeneralSettingsConfiguration::Configure,
                    dialog,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3));

            functionHandler.RegisterFunctionCall(showDialogFunction);
            configurableDisplays.RegisterDisplay(dialog);
            commandHandlers.RegisterHandler(dialog);
        }
    } // namespace Euroscope
} // namespace UKControllerPlugin
