#pragma once
#include "metar/MetarEventHandlerInterface.h"
#include "websocket/WebsocketEventProcessorInterface.h"
#include "minstack/MinStackLevel.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace TaskManager {
        class TaskRunnerInterface;
    }  // namespace TaskManager
    class HelperFunctions;
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace MinStack {
        class TerminalControlArea;
    }  // namespace MinStack
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace MinStack {

        /*
            Class for handling Minimum Stack Level Calculations
            and display.
        */
        class MinStackManager : public UKControllerPlugin::Metar::MetarEventHandlerInterface,
            public UKControllerPlugin::Websocket::WebsocketEventProcessorInterface
        {
            public:
                void AcknowledgeMsl(std::string key);
                void AddMsl(std::string key, std::string type, std::string name, unsigned int msl);
                void AddTerminalControlArea(std::shared_ptr<TerminalControlArea> tma);
                std::set<std::string> GetAllMslKeys(void) const;
                std::vector<std::shared_ptr<const TerminalControlArea>> GetAllTmas(void);
                const UKControllerPlugin::MinStack::MinStackLevel & GetMinStackLevel(std::string key) const;
                std::string GetMslKeyAirfield(std::string airfield) const;
                std::string GetMslKeyTma(std::string tma) const;
                std::string GetNameFromKey(std::string key) const;
                bool HasTerminalControlArea(std::string tma);
                virtual bool IsConcernedAirfield(std::string airfield);
                void MinStackClicked(const char* airfield);
                virtual void NewMetar(std::string station, std::string metar);
                int ProcessMetar(std::string metar);
                void SetMinStackLevel(std::string key, unsigned int msl);
                void RemoveTerminalControlArea(std::string tma);
                void UpdateAllMsls(nlohmann::json mslData);

                // Inherited via WebsocketEventProcessorInterface
                void ProcessWebsocketMessage(const UKControllerPlugin::Websocket::WebsocketMessage & message) override;
                std::set<UKControllerPlugin::Websocket::WebsocketSubscription> GetSubscriptions(void) const override;

                // What to return if an MSL is invalid
                const UKControllerPlugin::MinStack::MinStackLevel invalidMsl = {};

            private:

                std::shared_ptr<TerminalControlArea> GetTerminalControlAreaForAirfield(std::string airfield);

                // Map of TMA identifier -> TerminalControlArea
                std::map <std::string, std::shared_ptr<TerminalControlArea>> tmaList;

                // Vector that can conveniently be iterated over to render the MSL window.
                std::vector<std::shared_ptr<const TerminalControlArea>> tmaRenderingList;

                // Map of identifier to MSL
                std::map<std::string, UKControllerPlugin::MinStack::MinStackLevel> mslMap;
        };
    }  // namespace MinStack
}  // namespace UKControllerPlugin
