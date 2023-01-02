#pragma once
#include "euroscope/RunwayDialogAwareInterface.h"

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockRunwayDialogAwareInterface : public UKControllerPlugin::Euroscope::RunwayDialogAwareInterface
        {
            public:
            MockRunwayDialogAwareInterface();
            virtual ~MockRunwayDialogAwareInterface();
            MOCK_METHOD0(RunwayDialogSaved, void(void));
        };
    } // namespace Euroscope
} // namespace UKControllerPluginTest
