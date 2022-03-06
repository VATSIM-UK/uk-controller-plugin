#pragma once
#include "api/ApiSettingsProviderInterface.h"

using UKControllerPluginUtils::Api::ApiSettings;
using UKControllerPluginUtils::Api::ApiSettingsProviderInterface;

namespace UKControllerPluginUtilsTest::Api {
    class MockApiSettingsProvider : public ApiSettingsProviderInterface
    {
        public:
        MOCK_METHOD(ApiSettings&, Get, (), (override));
        MOCK_METHOD(bool, Has, (), (override));
        MOCK_METHOD(bool, Reload, (), (override));
    };
} // namespace UKControllerPluginUtilsTest::Api
