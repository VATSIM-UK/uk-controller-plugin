#pragma once
#include "metar/MetarEventHandlerInterface.h"

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
        class MinStackManager : public UKControllerPlugin::Metar::MetarEventHandlerInterface
        {
            public:
                void AddTerminalControlArea(std::shared_ptr<TerminalControlArea> tma);
                std::vector<std::shared_ptr<const TerminalControlArea>> GetAllTmas(void);
                bool HasTerminalControlArea(std::string tma);
                virtual bool IsConcernedAirfield(std::string airfield);
                void MinStackClicked(const char* airfield);
                virtual void NewMetar(std::string station, std::string metar);
                int ProcessMetar(std::string metar);
                void RemoveTerminalControlArea(std::string tma);

            private:
                std::shared_ptr<TerminalControlArea> GetTerminalControlAreaForAirfield(std::string airfield);

                // Map of TMA identifier -> TerminalControlArea
                std::map <std::string, std::shared_ptr<TerminalControlArea>> tmaList;

                // Vector that can conveniently be iterated over to render the MSL window.
                std::vector<std::shared_ptr<const TerminalControlArea>> tmaRenderingList;
        };
    }  // namespace MinStack
}  // namespace UKControllerPlugin
