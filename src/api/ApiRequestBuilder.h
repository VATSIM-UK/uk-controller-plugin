#pragma once
#include "curl/CurlRequest.h"

namespace UKControllerPlugin {
    namespace Api {

        /*
            Responsible for converting a set of input data into a cURL request
            that can sent to the API.
        */
        class ApiRequestBuilder
        {
            public:
                ApiRequestBuilder(std::string apiDomain, std::string apiKey);
                UKControllerPlugin::Curl::CurlRequest BuildAuthCheckRequest(void) const;
                UKControllerPlugin::Curl::CurlRequest BuildDependencyListRequest(void) const;
                UKControllerPlugin::Curl::CurlRequest BuildGetUriRequest(
                    std::string uri
                ) const;
                UKControllerPlugin::Curl::CurlRequest BuildSquawkAssignmentCheckRequest(std::string callsign) const;
                UKControllerPlugin::Curl::CurlRequest BuildSquawkAssignmentDeletionRequest(std::string callsign) const;
                UKControllerPlugin::Curl::CurlRequest BuildLocalSquawkAssignmentRequest(
                    std::string callsign,
                    std::string unit,
                    std::string flightRules
                ) const;
                UKControllerPlugin::Curl::CurlRequest BuildGeneralSquawkAssignmentRequest(
                    std::string callsign,
                    std::string origin,
                    std::string destination
                ) const;
                UKControllerPlugin::Curl::CurlRequest BuildHoldDependencyRequest(void) const;
                UKControllerPlugin::Curl::CurlRequest BuildUserHoldProfilesRequest(void) const;
                UKControllerPlugin::Curl::CurlRequest BuildDeleteUserHoldProfileRequest(unsigned int id) const;
                UKControllerPlugin::Curl::CurlRequest BuildCreateUserHoldProfileRequest(
                    std::string profileName,
                    std::set<unsigned int> holdIds
                ) const;
                UKControllerPlugin::Curl::CurlRequest BuildUpdateUserHoldProfileRequest(
                    unsigned int profileId,
                    std::string profileName,
                    std::set<unsigned int> holdIds
                ) const;
                UKControllerPlugin::Curl::CurlRequest BuildRemoteFileRequest(std::string uri) const;
                UKControllerPlugin::Curl::CurlRequest BuildVersionCheckRequest(std::string versionString) const;
                UKControllerPlugin::Curl::CurlRequest BuildWebsocketChannelAuthRequest(
                    std::string socketId,
                    std::string channel
                ) const;
                UKControllerPlugin::Curl::CurlRequest BuildMinStackLevelRequest(void) const;
                std::string GetApiDomain(void) const;
                std::string GetApiKey(void) const;
                void SetApiDomain(std::string domain);
                void SetApiKey(std::string key);

                // The type string to send in the payload if we want a general squawk
                const std::string generalSquawkAssignmentType = "general";

                // The type string to send in the payload if we want a local squawk
                const std::string localSquawkAssignmentType = "local";

            private:

                UKControllerPlugin::Curl::CurlRequest AddCommonHeaders(
                    UKControllerPlugin::Curl::CurlRequest request
                ) const;

                // The base URL of the API
                std::string apiDomain;

                // Our API key
                std::string apiKey;
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
