#pragma once
#include "euroscope/UserSettingProviderInterface.h"

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockUserSettingProviderInterface : public UKControllerPlugin::Euroscope::UserSettingProviderInterface
        {
            public:
            MOCK_METHOD1(GetKey, std::string(std::string));
            MOCK_METHOD1(KeyExists, bool(std::string));
            MOCK_METHOD3(SetKey, void(std::string, std::string, std::string));
        };
    } // namespace Euroscope
} // namespace UKControllerPluginTest
