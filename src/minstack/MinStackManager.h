#pragma once
#include "push/PushEventProcessorInterface.h"
#include "minstack/MinStackLevel.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace TaskManager {
        class TaskRunnerInterface;
    }  // namespace TaskManager
    class HelperFunctions;
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace MinStack {

        /*
            Class for handling Minimum Stack Level Calculations
            and display.
        */
        class MinStackManager : public Push::PushEventProcessorInterface
        {
            public:
                void AcknowledgeMsl(std::string key);
                void AddMsl(std::string key, std::string type, std::string name, unsigned int msl);
                std::set<std::string> GetAllMslKeys(void) const;
                const UKControllerPlugin::MinStack::MinStackLevel & GetMinStackLevel(std::string key) const;
                std::string GetMslKeyAirfield(std::string airfield) const;
                std::string GetMslKeyTma(std::string tma) const;
                std::string GetNameFromKey(std::string key) const;
                int ProcessMetar(std::string metar);
                void SetMinStackLevel(std::string key, unsigned int msl);
                void UpdateAllMsls(nlohmann::json mslData);
                void ProcessPushEvent(const Push::PushEvent& message) override;
                std::set<Push::PushEventSubscription> GetPushEventSubscriptions(void) const override;

                // What to return if an MSL is invalid
                const UKControllerPlugin::MinStack::MinStackLevel invalidMsl = {};

            private:

                // Map of identifier to MSL
                std::map<std::string, UKControllerPlugin::MinStack::MinStackLevel> mslMap;
        };
    }  // namespace MinStack
}  // namespace UKControllerPlugin
