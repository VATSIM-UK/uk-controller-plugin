#pragma once
#include "api/AbstractApiRequestPerformerFactory.h"

using UKControllerPluginUtils::Api::AbstractApiRequestPerformerFactory;
using UKControllerPluginUtils::Api::ApiRequestPerformerInterface;
using UKControllerPluginUtils::Api::ApiSettings;

namespace UKControllerPluginUtilsTest::Api {
    class MockApiRequestPerformerFactory : public AbstractApiRequestPerformerFactory
    {
        public:
        MOCK_METHOD(ApiRequestPerformerInterface&, Make, (const ApiSettings&), ());
    };
} // namespace UKControllerPluginUtilsTest::Api
