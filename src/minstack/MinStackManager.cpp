#include "pch/stdafx.h"
#include "minstack/MinStackManager.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::TaskManager::TaskRunnerInterface;
using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::Push::PushEvent;

namespace UKControllerPlugin {
    namespace MinStack {

        void MinStackManager::AcknowledgeMsl(std::string key)
        {
            if (!this->mslMap.count(key)) {
                return;
            }

            this->mslMap.at(key).Acknowledge();
        }

        /*
            Add a MSL with the specified key
        */
        void MinStackManager::AddMsl(std::string key, std::string type, std::string name, unsigned int msl)
        {
            if (this->mslMap.count(key)) {
                return;
            }

            this->mslMap[key] = {
                type,
                name,
                msl
            };
        }

        /*
            Get all keys for MSLs
        */
        std::set<std::string> MinStackManager::GetAllMslKeys(void) const
        {
            std::set<std::string> keys;
            for (
                std::map<std::string, MinStackLevel>::const_iterator it = this->mslMap.cbegin();
                it != this->mslMap.cend();
                ++it
            ) {
                keys.insert(it->first);
            }

            return keys;
        }

        /*
            Get the selected minstack level
        */
        const MinStackLevel & MinStackManager::GetMinStackLevel(std::string key) const
        {
            return this->mslMap.count(key)
                ? this->mslMap.at(key)
                : this->invalidMsl;
        }

        std::string MinStackManager::GetMslKeyAirfield(std::string airfield) const
        {
            return "airfield." + airfield;
        }

        std::string MinStackManager::GetMslKeyTma(std::string tma) const
        {
            return "tma." + tma;
        }

        /*
            Get the name part of the key
        */
        std::string MinStackManager::GetNameFromKey(std::string key) const
        {
            return key.substr(key.find('.') + 1);
        }

        /*
            We've received some new MSLs from the web API, update them locally
        */
        void MinStackManager::ProcessPushEvent(const PushEvent& message)
        {
            if (message.event != "App\\Events\\MinStacksUpdatedEvent") {
                return;
            }

            this->UpdateAllMsls(message.data);
        }

        std::set<PushEventSubscription> MinStackManager::GetPushEventSubscriptions(void) const
        {
            return {
                {
                    PushEventSubscription::SUB_TYPE_CHANNEL,
                    "private-minstack-updates"
                }
            };
        }

        /*
            When we receive a new metar, process it in tokens until we get the QNH.
        */
        int MinStackManager::ProcessMetar(std::string metar)
        {
            std::istringstream buf(metar);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> tokens(beg, end);

            for each (std::string token in tokens)
            {
                if (token[0] == 'Q' && HelperFunctions::IsAnInteger(token.substr(1))) {
                    return std::stoi(token.substr(1));
                }
            }

            return 0;
        }

        /*
            Set the MSL for the given key
        */
        void MinStackManager::SetMinStackLevel(std::string key, unsigned int msl)
        {
            if (!this->mslMap.count(key)) {
                return;
            }

            this->mslMap.at(key).msl = msl;
            this->mslMap.at(key).updatedAt = std::chrono::system_clock::now();
        }

        void MinStackManager::UpdateAllMsls(nlohmann::json mslData)
        {
            // Update the airfield MSLs
            if (mslData.count("airfield")) {
                for (
                    nlohmann::json::const_iterator airfieldIt = mslData["airfield"].cbegin();
                    airfieldIt != mslData["airfield"].cend();
                    ++airfieldIt
                ) {
                    if (airfieldIt.value().is_null()) {
                        continue;
                    }

                    this->mslMap[this->GetMslKeyAirfield(airfieldIt.key())] = {
                        "airfield",
                        airfieldIt.key(),
                        airfieldIt.value().get<unsigned int>()
                    };
                }
            }

            // Update the TMA MSLs
            if (mslData.count("tma")) {
                for (
                    nlohmann::json::const_iterator tmaIt = mslData["tma"].cbegin();
                    tmaIt != mslData["tma"].cend();
                    ++tmaIt
                ) {
                    if (tmaIt.value().is_null()) {
                        continue;
                    }

                    this->mslMap[this->GetMslKeyTma(tmaIt.key())] = {
                        "tma",
                        tmaIt.key(),
                        tmaIt.value().get<unsigned int>()
                    };
                }
            }
        }
    }  // namespace MinStack
}  // namespace UKControllerPlugin
