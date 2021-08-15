#include "pch/pch.h"

#include "helper/HelperFunctions.h"
#include "minstack/MinStackManager.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;

namespace UKControllerPlugin::MinStack {

    MinStackManager::MinStackManager()
        : invalidMsl({})
    {
    
    }
    
    void MinStackManager::AcknowledgeMsl(const std::string& key)
    {
        if (this->mslMap.count(key) == 0) {
            return;
        }

        this->mslMap.at(key).Acknowledge();
    }

    /*
        Add a MSL with the specified key
    */
    void MinStackManager::AddMsl(const std::string& key, std::string type, std::string name, unsigned int msl)
    {
        if (this->mslMap.count(key) == 1) {
            return;
        }

        this->mslMap[key] = {std::move(type), std::move(name), msl};
    }

    /*
        Get all keys for MSLs
    */
    auto MinStackManager::GetAllMslKeys() const -> std::set<std::string>
    {
        std::set<std::string> keys;
        for (const auto & it : this->mslMap) {
            keys.insert(it.first);
        }

        return keys;
    }

    /*
        Get the selected minstack level
    */
    auto MinStackManager::GetMinStackLevel(const std::string& key) const -> const MinStackLevel&
    {
        return this->mslMap.count(key) == 1 ? this->mslMap.at(key) : this->invalidMsl;
    }

    auto MinStackManager::GetMslKeyAirfield(const std::string& airfield) -> std::string
    {
        return "airfield." + airfield;
    }

    auto MinStackManager::GetMslKeyTma(const std::string& tma) -> std::string
    {
        return "tma." + tma;
    }

    /*
        Get the name part of the key
    */
    auto MinStackManager::GetNameFromKey(const std::string& key) -> std::string
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

    auto MinStackManager::GetPushEventSubscriptions() const -> std::set<PushEventSubscription>
    {
        return {{PushEventSubscription::SUB_TYPE_CHANNEL, "private-minstack-updates"}};
    }

    void MinStackManager::PluginEventsSynced()
    {
        // Nothing here
    }

    /*
        Set the MSL for the given key
    */
    void MinStackManager::SetMinStackLevel(const std::string& key, unsigned int msl)
    {
        if (this->mslMap.count(key) == 0) {
            return;
        }

        this->mslMap.at(key).msl = msl;
        this->mslMap.at(key).updatedAt = std::chrono::system_clock::now();
    }

    void MinStackManager::UpdateAllMsls(nlohmann::json mslData)
    {
        // Update the airfield MSLs
        if (mslData.count("airfield") != 0) {
            for (nlohmann::json::const_iterator airfieldIt = mslData["airfield"].cbegin();
                 airfieldIt != mslData["airfield"].cend();
                 ++airfieldIt) {
                if (airfieldIt.value().is_null()) {
                    continue;
                }

                this->mslMap[this->GetMslKeyAirfield(airfieldIt.key())] = {
                    "airfield", airfieldIt.key(), airfieldIt.value().get<unsigned int>()};
            }
        }

        // Update the TMA MSLs
        if (mslData.count("tma") != 0) {
            for (nlohmann::json::const_iterator tmaIt = mslData["tma"].cbegin(); tmaIt != mslData["tma"].cend();
                 ++tmaIt) {
                if (tmaIt.value().is_null()) {
                    continue;
                }

                this->mslMap[this->GetMslKeyTma(tmaIt.key())] = {"tma", tmaIt.key(), tmaIt.value().get<unsigned int>()};
            }
        }
    }

    auto MinStackManager::InvalidMsl() const -> const MinStackLevel&
    {
        return this->invalidMsl;
    }
} // namespace UKControllerPlugin::MinStack
