#pragma once
#include "historytrail/HistoryTrailDialog.h"

namespace UKControllerPluginTest {
    namespace HistoryTrail {
        class MockHistoryTrailDialog : public UKControllerPlugin::HistoryTrail::HistoryTrailDialog
        {
            public:
            MockHistoryTrailDialog(void) : HistoryTrailDialog()
            {
            }
        };
    } // namespace HistoryTrail
} // namespace UKControllerPluginTest
