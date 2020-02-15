#pragma once
#include "websocket/WebsocketEventProcessorInterface.h"
#include "regional/RegionalPressure.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace TaskManager {
        class TaskRunnerInterface;
    }  // namespace TaskManager
    class HelperFunctions;
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Regional {

        /*
            Class for storing of regional pressure settingsd
        */
        class RegionalPressureManager : public UKControllerPlugin::Websocket::WebsocketEventProcessorInterface
        {
            public:
                RegionalPressureManager(void);
                explicit RegionalPressureManager(std::map<std::string, std::string> keyMap);
                void AcknowledgePressure(std::string key);
                void AddRegionalPressure(std::string key, std::string name, unsigned int pressure);
                size_t CountAltimeterSettingRegions(void) const;
                std::set<std::string> GetAllRegionalPressureKeys(void) const;
                const UKControllerPlugin::Regional::RegionalPressure & GetRegionalPressure(std::string key) const;
                std::string GetNameFromKey(std::string key) const;
                void SetPressure(std::string key, unsigned int pressure);
                void UpdateAllPressures(nlohmann::json pressureData);

                // Inherited via WebsocketEventProcessorInterface
                void ProcessWebsocketMessage(const UKControllerPlugin::Websocket::WebsocketMessage & message) override;
                std::set<UKControllerPlugin::Websocket::WebsocketSubscription> GetSubscriptions(void) const override;

                // What to return if an RPS is invalid
                const UKControllerPlugin::Regional::RegionalPressure invalidPressure = {};

            private:

                // Map of identifier to rps
                std::map<std::string, UKControllerPlugin::Regional::RegionalPressure> pressureMap;

                // Maps the ASR key to its name
                const std::map<std::string, std::string> keyMap;
        };
    }  // namespace Regional
}  // namespace UKControllerPlugin
