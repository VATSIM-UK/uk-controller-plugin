#include "pch/stdafx.h"
#include "minstack/MinStackManager.h"
#include "minstack/TerminalControlArea.h"
#include "helper/HelperFunctions.h"
#include "task/TaskRunner.h"

using UKControllerPlugin::TaskManager::TaskRunnerInterface;
using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPlugin::Websocket::WebsocketMessage;

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
            Adds a TMA to the list of TMAs.
        */
        void MinStackManager::AddTerminalControlArea(std::shared_ptr<TerminalControlArea> tma)
        {
            // Add it to the list
            if (this->HasTerminalControlArea(tma->GetCharName())) {
                return;
            }

            this->tmaList[tma->GetCharName()] = tma;
            this->tmaRenderingList.push_back(tma);
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
            Returns a vector of pointers to the constant versions of the TMAs.
        */
        std::vector<std::shared_ptr<const TerminalControlArea>> MinStackManager::GetAllTmas(void)
        {
            return this->tmaRenderingList;
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
            Returns a pointer to the TMA that is related to the given airfield
        */
        std::shared_ptr<TerminalControlArea> MinStackManager::GetTerminalControlAreaForAirfield(std::string airfield)
        {
            for (
                std::map<std::string, std::shared_ptr<TerminalControlArea>>::iterator it = this->tmaList.begin();
                it != this->tmaList.end();
                ++it
            ) {
                if (it->second->GetCalculationAirfield().compare(airfield) == 0) {
                    return it->second;
                }
            }

            return NULL;
        }

        /*
            We've received some new MSLs from the web API, update them locally
        */
        void MinStackManager::ProcessWebsocketMessage(const WebsocketMessage & message)
        {
            if (message.event != "App\\Events\\MinStacksUpdatedEvent") {
                return;
            }

            this->UpdateAllMsls(message.data);
        }

        std::set<WebsocketSubscription> MinStackManager::GetSubscriptions(void) const
        {
            return {
                {
                    WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-minstack-updates"
                }
            };
        }

        /*
            Returns true if the TMA is known to the class.
        */
        bool MinStackManager::HasTerminalControlArea(std::string tma)
        {
            return this->tmaList.count(tma) == 1;
        }

        /*
            Returns true if one of the TMAs managed by this
            class has it's MSL determined by the given airfield.
        */
        bool MinStackManager::IsConcernedAirfield(std::string airfield)
        {
            for (
                std::map<std::string, std::shared_ptr<TerminalControlArea>>::iterator it = this->tmaList.begin();
                it != this->tmaList.end();
                ++it
            ) {
                if (it->second->GetCalculationAirfield().compare(airfield) == 0) {
                    return true;
                }
            }

            return false;
        }

        /*
            The user has clicked to acknowledge one of the
            minimum stack levels, set the TMA to acknowledged.
        */
        void MinStackManager::MinStackClicked(const char* tma)
        {
            if (!this->HasTerminalControlArea(tma)) {
                return;
            }

            this->tmaList.at(tma)->AcknowledgeMinStack();
        }

        /*
            A new QNH has been detected, so we check the TMA exists
            then set the new MSL.
        */
        void MinStackManager::NewMetar(std::string station, std::string metar)
        {
            if (!this->IsConcernedAirfield(station)) {
                return;
            }

            int qnh = this->ProcessMetar(metar);
            if (qnh) {
                GetTerminalControlAreaForAirfield(station)->SetMinStack(qnh);
            }
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

        /*
            Removes a given TMA from the MinStack module.
        */
        void MinStackManager::RemoveTerminalControlArea(std::string tma)
        {
            if (this->HasTerminalControlArea(tma)) {
                return;
            }

            this->tmaList.erase(tmaList.find(tma));
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
