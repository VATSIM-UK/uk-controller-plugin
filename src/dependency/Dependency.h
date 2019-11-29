#pragma once

namespace UKControllerPlugin {
    namespace Dependency {

        /*
            All the dependencies that this version of the plugin
            knows about - others may exist and will be downloaded anyway.
        */
        enum Dependency
        {
            aircraft,
            sids,
            airfields,
            controllers,
            handoffs,
            holds,
            holdProfiles,
            wake,
            prenotes
        };
    }  // namespace Dependency
}  // namespace UKControllerPlugin
