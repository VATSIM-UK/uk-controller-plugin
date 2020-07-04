#pragma once
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "mock/MockFlightplanRadarTargetPair.h"

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
                void AddAllFlightplansItem(UKControllerPluginTest::Euroscope::MockFlightplanRadarTargetPair item)
                {
                    this->allFpRtPairs.push_back(item);
                }
                MOCK_METHOD1(AddItemToPopupList, void(const UKControllerPlugin::Plugin::PopupMenuItem item));
                MOCK_CONST_METHOD0(GetEuroscopeConnectionStatus, int(void));
                MOCK_CONST_METHOD1(GetDistanceFromUserVisibilityCentre, double(EuroScopePlugIn::CPosition position));
                MOCK_CONST_METHOD0(
                    GetUserControllerObject,
                    std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCControllerInterface>(void)
                );
                MOCK_CONST_METHOD1(
                    GetFlightplanForCallsign,
                    std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>(std::string)
                );
                MOCK_CONST_METHOD1(
                    GetRadarTargetForCallsign,
                    std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface>(std::string)
                );
                MOCK_CONST_METHOD0(
                    GetSelectedFlightplan,
                    std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>()
                );
                MOCK_CONST_METHOD0(
                    GetSelectedRadarTarget,
                    std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface>()
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

                void ApplyFunctionToAllFlightplans(
                    std::function<
                    void(
                    std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>,
                    std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface>
                )
                    > function
                ) override
                {
                    for (
                        std::list<UKControllerPluginTest::Euroscope::MockFlightplanRadarTargetPair>::const_iterator it =
                            this->allFpRtPairs.cbegin();
                        it != this->allFpRtPairs.cend();
                        ++it
                    ) {
                        function(it->fp, it->rt);
                    }
                };

            private:
                std::list<UKControllerPluginTest::Euroscope::MockFlightplanRadarTargetPair> allFpRtPairs;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
