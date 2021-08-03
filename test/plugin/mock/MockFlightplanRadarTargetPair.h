#pragma once
#include "pch/pch.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"

namespace UKControllerPluginTest {
    namespace Euroscope {
        typedef struct MockFlightplanRadarTargetPair {
            std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface> fp;
            std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface> rt;
        } MockFlightplanRadarTargetPair;
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
