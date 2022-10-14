#pragma once
#include "prenote/PrenoteUserRelevanceChecker.h"

namespace UKControllerPluginTest::Prenote {
    class MockPrenoteUserRelevanceChecker : public UKControllerPlugin::Prenote::PrenoteUserRelevanceChecker
    {
        public:
        MOCK_METHOD(bool, IsRelevant, (const PrenoteMessage&), (const, override));
    };
} // namespace UKControllerPluginTest::Prenote
