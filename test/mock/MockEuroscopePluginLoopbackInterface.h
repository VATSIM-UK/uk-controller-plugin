#pragma once
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPluginTest {
    namespace Euroscope {
        class MockEuroscopePluginLoopbackInterface :
            public UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface
        {
            public:
                MOCK_METHOD1(AddItemToPopupList, void(const UKControllerPlugin::Plugin::PopupMenuItem item));
                MOCK_CONST_METHOD0(GetEuroscopeConnectionStatus, int(void));
                MOCK_CONST_METHOD1(GetDistanceFromUserVisibilityCentre, double(EuroScopePlugIn::CPosition position));
                MOCK_CONST_METHOD1(
                    GetFlightplanForCallsign,
                    std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>(std::string)
                );
                MOCK_CONST_METHOD1(
                    GetRadarTargetForCallsign,
                    std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface>(std::string)
                );
                MOCK_METHOD3(TriggerPopupList, void(RECT area, std::string title, int numColumns));
                MOCK_METHOD2(RegisterTagFunction, void(int, std::string));
                MOCK_METHOD2(RegisterTagItem, void(int, std::string));
                MOCK_METHOD1(TriggerFlightplanUpdateForCallsign, void(std::string));
                MOCK_METHOD8(
                    ChatAreaMessage,
                    void(
                        std::string handler,
                        std::string sender,
                        std::string message,
                        bool showHandler,
                        bool markUnread,
                        bool overrideBusy,
                        bool flash,
                        bool confirm
                    )
                );
        };
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
