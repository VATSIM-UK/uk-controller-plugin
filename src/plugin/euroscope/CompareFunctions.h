#pragma once

namespace UKControllerPlugin {
    namespace Tag {
        struct TagFunction;
    } // namespace Tag
    namespace Euroscope {
        struct CallbackFunction;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            Used for comparing CallbackFunction and TagFunction
            structs when doing set operations.
        */
        typedef struct CompareFunctions
        {
            using is_transparent = int;

            bool operator()(const UKControllerPlugin::Euroscope::CallbackFunction& callback, int id) const;
            bool operator()(int id, const UKControllerPlugin::Euroscope::CallbackFunction& callback) const;
            bool operator()(
                const UKControllerPlugin::Euroscope::CallbackFunction& a,
                const UKControllerPlugin::Euroscope::CallbackFunction& b) const;
            bool operator()(const UKControllerPlugin::Tag::TagFunction& callback, int id) const;
            bool operator()(int id, const UKControllerPlugin::Tag::TagFunction& callback) const;
            bool operator()(
                const UKControllerPlugin::Tag::TagFunction& a, const UKControllerPlugin::Tag::TagFunction& b) const;

        } CompareFunctions;
    } // namespace Euroscope
} // namespace UKControllerPlugin
