#include "pch/stdafx.h"
#include "stands/StandEventHandler.h"

namespace UKControllerPlugin {
    namespace Stands {

        StandEventHandler::StandEventHandler(const std::map<std::string, std::set<Stand, CompareStands>> stands)
            : stands(stands)
        {
        }
    }  // namespace Stands
}  // namespace UKControllerPlugin
