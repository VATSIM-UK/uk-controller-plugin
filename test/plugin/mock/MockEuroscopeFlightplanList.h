#pragma once
#include "euroscope/EuroscopeFlightplanListInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockEuroscopeFlightplanList : public UKControllerPlugin::Euroscope::EuroscopeFlightplanListInterface
        {
            public:
            MOCK_METHOD(int, NumberOfColumns, (), (override));
            MOCK_METHOD(
                void,
                AddColumn,
                (std::string, int, bool, std::string, int, std::string, int, std::string, int),
                (override));
            MOCK_METHOD(
                void, AddFlightplan, (UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&), (override));
            MOCK_METHOD(
                void,
                AddFlightplan,
                (std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>),
                (override));
            MOCK_METHOD(
                void, RemoveFlightplan, (UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&), (override));
            MOCK_METHOD(
                void,
                RemoveFlightplan,
                (std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>),
                (override));
            MOCK_METHOD(void, Show, (), (override));
            MOCK_METHOD(void, Hide, (), (override));
        };
    } // namespace Euroscope
} // namespace UKControllerPluginTest
