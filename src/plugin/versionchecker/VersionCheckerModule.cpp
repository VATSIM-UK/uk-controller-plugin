#include "VersionCheckerModule.h"
#include "ControllerPackVersion.h"
#include "EuroscopeVersion.h"
#include "SectorFileProviderVersion.h"
#include "SendUpdateRequiredUserMessage.h"
#include "UpdateRequiredEvent.h"
#include "VersionInterface.h"
#include "bootstrap/PersistenceContainer.h"
#include "eventhandler/EventBus.h"
#include "log/LoggerFunctions.h"
#include "task/TaskRunnerInterface.h"

using UKControllerPluginUtils::EventHandler::EventBus;
using UKControllerPluginUtils::EventHandler::EventHandlerFlags;

namespace UKControllerPlugin::VersionChecker {

    void VersionCheckerModule::BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        EventBus::Bus().AddHandler<UpdateRequiredEvent>(
            std::make_shared<SendUpdateRequiredUserMessage>(*container.plugin), EventHandlerFlags::EuroscopeThread);

        container.taskRunner->QueueAsynchronousTask([&container]() {
            LogInfo("Starting version checking task");

            Check(ControllerPackVersion(*container.curl, *container.windows), "pack");
            Check(EuroscopeVersion(), "euroscope");
            Check(SectorFileProviderVersion(*container.windows), "sector file provider");
        });
    }

    void VersionCheckerModule::Check(VersionInterface&& iface, const char* label)
    {
        std::string current, latest;

        if ((current = GetVersion([&iface]() { return iface.GetCurrentVersion(); }, "current")).empty() ||
            (latest = GetVersion([&iface]() { return iface.GetLatestVersion(); }, "latest")).empty()) {
            LogWarning(std::format("Skipping version check for {}", label));
            return;
        }

        if (iface.IsUpdateRequired(current, latest)) {
            auto message = iface.GetUpdateMessage();
            EventBus::Bus().OnEvent<UpdateRequiredEvent>({message, current, latest});
        }
    }

    auto VersionCheckerModule::GetVersion(std::function<std::string()> function, const char* label) -> std::string
    {
        std::string version;

        try {
            version = function();
        } catch (const std::exception& e) {
            LogError(std::format("Exception checking {} version: {}", label, e.what()));
            return "";
        }

        if (!version.empty()) {
            const char* TRIM_CHARS = "\t\n\r ";

            version.erase(0, version.find_first_not_of(TRIM_CHARS));
            version.erase(version.find_last_not_of(TRIM_CHARS) + 1);
        }

        return version;
    }

} // namespace UKControllerPlugin::VersionChecker
