#include "AbstractPrenote.h"
#include "PrenoteFactory.h"
#include "PrenoteService.h"
#include "PrenoteServiceFactory.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "controller/ActiveCallsignCollection.h"
#include "message/UserMessager.h"
#include "ownership/AirfieldOwnershipManager.h"

using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Prenote::PrenoteFactory;
using UKControllerPlugin::Prenote::PrenoteService;

namespace UKControllerPlugin::Prenote {

    PrenoteServiceFactory::PrenoteServiceFactory(const PrenoteFactory& prenoteFactory, UserMessager& userMessager)
        : prenoteFactory(prenoteFactory), userMessager(userMessager)
    {
    }

    /*
        Creates a prenote event handler from the specification
    */
    auto PrenoteServiceFactory::Create(
        const UKControllerPlugin::Ownership::AirfieldOwnershipManager& airfieldOwnership,
        const UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns,
        const nlohmann::json& json) -> std::unique_ptr<PrenoteService>
    {
        std::unique_ptr<PrenoteService> handler =
            std::make_unique<PrenoteService>(airfieldOwnership, activeCallsigns, userMessager);

        if (!json.is_array()) {
            LogError("Prenote data file not an array, prenotes not loaded");
            BootstrapWarningMessage message("Prenote data file not an array, prenotes not loaded");
            this->userMessager.SendMessageToUser(message);
            throw std::invalid_argument("Prenotes not an array");
        }

        if (json.empty()) {
            LogError("Prenote data array empty, prenotes not loaded");
            BootstrapWarningMessage message("Prenote data array empty, prenotes not loaded");
            this->userMessager.SendMessageToUser(message);
            throw std::invalid_argument("Prenotes empty");
        }

        for (nlohmann::json::const_iterator it = json.cbegin(); it != json.cend(); ++it) {
            try {
                handler->AddPrenote(this->prenoteFactory.CreateFromJson(*it));
            } catch (std::invalid_argument& invalid) {
                // A dodgy prenote isn't a reason to stop, but we should tell the user
                BootstrapWarningMessage message(invalid.what());
                this->userMessager.SendMessageToUser(message);
                std::string what = invalid.what();
                LogError("Invalid prenote detected: " + what);
            } catch (nlohmann::json::exception&) {
                BootstrapWarningMessage message("JSON exception when loading prenotes: " + json.dump());
                this->userMessager.SendMessageToUser(message);
            }
        }

        LogInfo("Loaded " + std::to_string(handler->CountPrenotes()) + " prenotes");
        return handler;
    }
} // namespace UKControllerPlugin::Prenote
