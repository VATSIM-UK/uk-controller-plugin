#include "pch/stdafx.h"
#include "prenote/PrenoteServiceFactory.h"
#include "controller/ActiveCallsignCollection.h"
#include "airfield/AirfieldOwnershipManager.h"
#include "message/UserMessager.h"
#include "prenote/PrenoteService.h"
#include "prenote/PrenoteFactory.h"
#include "bootstrap/BootstrapWarningMessage.h"

using UKControllerPlugin::Airfield::AirfieldOwnershipManager;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Prenote::PrenoteService;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Prenote::PrenoteFactory;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;

namespace UKControllerPlugin {
    namespace Prenote {

        PrenoteServiceFactory::PrenoteServiceFactory(
            const PrenoteFactory & prenoteFactory,
            UserMessager & userMessager
        )
            : prenoteFactory(prenoteFactory), userMessager(userMessager)
        {

        }

        /*
            Creates a prenote event handler from the specification
        */
        std::unique_ptr<PrenoteService> PrenoteServiceFactory::Create(
            const UKControllerPlugin::Airfield::AirfieldOwnershipManager & airfieldOwnership,
            const UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns,
            const nlohmann::json & json
        ) {
            std::unique_ptr<PrenoteService> handler = std::make_unique<PrenoteService>(
                airfieldOwnership,
                activeCallsigns,
                userMessager
            );

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
                } catch (std::invalid_argument invalid) {
                    // A dodgy prenote isn't a reason to stop, but we should tell the user
                    BootstrapWarningMessage message(invalid.what());
                    this->userMessager.SendMessageToUser(message);
                    std::string what = invalid.what();
                    LogError("Invalid prenote detected: " + what);
                } catch (nlohmann::json::exception) {
                    BootstrapWarningMessage message("JSON exception when loading prenotes: " + json.dump());
                    this->userMessager.SendMessageToUser(message);
                }
            }

            LogInfo("Loaded " + std::to_string(handler->CountPrenotes()) + " prenotes");
            return handler;
        }
    }  // namespace Prenote
}  // namespace UKControllerPlugin
