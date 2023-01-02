#pragma once
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopeFlightplanListInterface.h"
#include "mock/MockFlightplanRadarTargetPair.h"
#include "mock/MockEuroScopeCControllerInterface.h"

namespace UKControllerPluginTest {
    namespace Euroscope {
        class MockEuroscopePluginLoopbackInterface
            : public UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface
        {
            public:
            MockEuroscopePluginLoopbackInterface();
            virtual ~MockEuroscopePluginLoopbackInterface();
            void AddAllFlightplansItem(MockFlightplanRadarTargetPair item);
            void
            AddAllControllersItem(std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCControllerInterface> item);

            MOCK_METHOD1(AddItemToPopupList, void(const UKControllerPlugin::Plugin::PopupMenuItem item));
            MOCK_CONST_METHOD0(GetEuroscopeConnectionStatus, int(void));
            MOCK_CONST_METHOD1(GetDistanceFromUserVisibilityCentre, double(EuroScopePlugIn::CPosition position));
            MOCK_CONST_METHOD0(
                GetUserControllerObject,
                std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCControllerInterface>(void));
            MOCK_CONST_METHOD1(
                GetFlightplanForCallsign,
                std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>(std::string));
            MOCK_CONST_METHOD1(
                GetRadarTargetForCallsign,
                std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface>(std::string));
            MOCK_CONST_METHOD0(
                GetSelectedFlightplan, std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>());
            MOCK_CONST_METHOD0(
                GetSelectedRadarTarget,
                std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface>());
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
                    bool confirm));
            MOCK_METHOD3(ShowTextEditPopup, void(RECT, int, std::string));
            MOCK_METHOD(
                std::shared_ptr<UKControllerPlugin::Euroscope::EuroscopeFlightplanListInterface>,
                RegisterFlightplanList,
                (std::string),
                (override));
            void ApplyFunctionToAllFlightplans(
                std::function<void(
                    std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>,
                    std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface>)> function) override;
            void ApplyFunctionToAllControllers(
                std::function<void(std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCControllerInterface>)>
                    function) override;
            MOCK_METHOD(
                void,
                SetEuroscopeSelectedFlightplanPointer,
                (std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>));
            MOCK_METHOD(
                void,
                SetEuroscopeSelectedFlightplanReference,
                (const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface&));
            void SetEuroscopeSelectedFlightplan(
                std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface> flightplan) override;
            void SetEuroscopeSelectedFlightplan(
                const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan) override;
            void ExpectNoFlightplanLoop();

            private:
            std::list<MockFlightplanRadarTargetPair> allFpRtPairs;
            std::list<std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCControllerInterface>> allControllers;

            bool expectFlightplanLoopNoFire = false;
        };
    } // namespace Euroscope
} // namespace UKControllerPluginTest
