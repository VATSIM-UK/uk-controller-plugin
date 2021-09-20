#pragma once

namespace UKControllerPlugin::Euroscope {

    /*
        An interface for classes that want to know when someone saves the EuroScope
        runway selection dialog.
    */
    class RunwayDialogAwareInterface
    {
        public:
        virtual void RunwayDialogSaved() = 0;
    };
} // namespace UKControllerPlugin::Euroscope
