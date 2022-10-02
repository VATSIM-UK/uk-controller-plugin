#pragma once
#include "historytrail/HistoryTrailRepository.h"

namespace UKControllerPluginTest {
    namespace HistoryTrail {
        class MockHistoryTrailRepository : public UKControllerPlugin::HistoryTrail::HistoryTrailRepository
        {
            public:
            MockHistoryTrailRepository() : HistoryTrailRepository()
            {
            }
        };
    } // namespace HistoryTrail
} // namespace UKControllerPluginTest
