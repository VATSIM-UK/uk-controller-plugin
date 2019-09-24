#pragma once
#include "euroscope/RunwayDialogAwareInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            A collection of objects that want to know
            when sector a file is updated or changed.
        */
        class RunwayDialogAwareCollection
        {
            public:

                void AddHandler(std::shared_ptr<UKControllerPlugin::Euroscope::RunwayDialogAwareInterface> handler);
                size_t CountHandlers(void) const;
                void RunwayDialogSave(void) const;

            private:
                
                std::set<std::shared_ptr<UKControllerPlugin::Euroscope::RunwayDialogAwareInterface>> handlers;
        };

    }  // namespace Euroscope
}  // namespace UKControllerPlugin
