#pragma once

namespace UKControllerPlugin {
    namespace Plugin {

        /*
            Contains the version details of the plugin
            that get passed to ES and are also used in update
            checks.
        */
        typedef struct PluginVersion {
            static const char * const version;
            static const char * const title;
            static const char * const copyright;
            static const char * const author;
        } PluginVersion;
    }  // namespace Plugin
}  // namespace UKControllerPlugin
