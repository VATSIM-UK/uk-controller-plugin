#pragma once
#include "pch/testingutilspch.h"
#include "api/ApiInterface.h"
#include "srd/SrdSearchParameters.h"

namespace UKControllerPluginTest {
    namespace Api {

        class MockApiInterface : public UKControllerPlugin::Api::ApiInterface
        {
            public:
                MOCK_CONST_METHOD3(
                    CreateGeneralSquawkAssignment,
                    UKControllerPlugin::Squawk::ApiSquawkAllocation (
                        std::string,
                        std::string,
                        std::string
                    )
                );
                MOCK_CONST_METHOD3(
                    CreateLocalSquawkAssignment,
                    UKControllerPlugin::Squawk::ApiSquawkAllocation (
                        std::string,
                        std::string,
                        std::string
                    )
                );
                MOCK_CONST_METHOD2(AuthoriseWebsocketChannel, std::string(std::string socketId, std::string channel));
                MOCK_CONST_METHOD0(CheckApiAuthorisation, bool(void));
                MOCK_CONST_METHOD1(DeleteSquawkAssignment, void(std::string));
                MOCK_CONST_METHOD0(GetDependencyList, nlohmann::json(void));
                MOCK_CONST_METHOD1(FetchRemoteFile, std::string(std::string));
                MOCK_CONST_METHOD1(GetAssignedSquawk, UKControllerPlugin::Squawk::ApiSquawkAllocation(std::string));
                MOCK_CONST_METHOD0(GetApiDomain, std::string(void));
                MOCK_CONST_METHOD0(GetApiKey, std::string(void));
                MOCK_CONST_METHOD0(GetHoldDependency, nlohmann::json(void));
                MOCK_CONST_METHOD0(GetAssignedHolds, nlohmann::json(void));
                MOCK_CONST_METHOD2(AssignAircraftToHold, void(std::string, std::string));
                MOCK_CONST_METHOD1(UnassignAircraftHold, void(std::string));
                MOCK_CONST_METHOD0(GetMinStackLevels, nlohmann::json(void));
                MOCK_CONST_METHOD1(GetUri, nlohmann::json(std::string uri));
                MOCK_CONST_METHOD0(GetRegionalPressures, nlohmann::json(void));
                MOCK_CONST_METHOD1(
                    SearchSrd,
                    nlohmann::json(UKControllerPlugin::Srd::SrdSearchParameters)
                );
                MOCK_CONST_METHOD0(GetAssignedStands, nlohmann::json(void));
                MOCK_CONST_METHOD2(AssignStandToAircraft, void(std::string, int));
                MOCK_CONST_METHOD1(DeleteStandAssignmentForAircraft, void(std::string));
                MOCK_CONST_METHOD4(SendEnrouteRelease, void(std::string, std::string, std::string, int));
                MOCK_CONST_METHOD5(
                    SendEnrouteReleaseWithReleasePoint,
                    void(std::string, std::string, std::string, int, std::string)
                );
                MOCK_CONST_METHOD0(GetAllNotifications, nlohmann::json(void));
                MOCK_CONST_METHOD0(GetUnreadNotifications, nlohmann::json(void));
                MOCK_CONST_METHOD1(ReadNotification, void(int));
                MOCK_CONST_METHOD0(SyncPluginEvents, nlohmann::json (void));
                MOCK_CONST_METHOD1(GetLatestPluginEvents, nlohmann::json(int));
                MOCK_CONST_METHOD1(UpdateCheck, int(std::string));
                MOCK_METHOD1(SetApiDomain, void(std::string));
                MOCK_METHOD1(SetApiKey, void(std::string));
                MOCK_CONST_METHOD0(GetUpdateDetails, nlohmann::json(void));
        };
    }  // namespace Api
}  // namespace UKControllerPluginTest
