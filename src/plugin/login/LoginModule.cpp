#include "bootstrap/PersistenceContainer.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "login/Login.h"
#include "login/LoginModule.h"
#include "plugin/UKPlugin.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::Login;
namespace UKControllerPlugin::Controller {

    /*
        Bootstrap the login module.
    */
    void LoginModule::BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer& persistence)
    {
        std::shared_ptr<Login> handler = std::make_shared<Login>(*persistence.plugin, *persistence.controllerHandler);

        persistence.login = handler;
        persistence.timedHandler->RegisterEvent(handler, 2);
        persistence.controllerHandler->RegisterHandler(handler);
    }
} // namespace UKControllerPlugin::Controller
