#pragma once

namespace UKControllerPlugin {
    namespace Dialog {
        struct DialogData;
    } // namespace Dialog
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Dialog {

        /*
            Used for comparing DialogData structs so that they can
            be accessed by integer ids
        */
        typedef struct CompareDialogs
        {
            using is_transparent = unsigned int;

            bool operator()(const UKControllerPlugin::Dialog::DialogData& hold, unsigned int id) const;
            bool operator()(unsigned int, const UKControllerPlugin::Dialog::DialogData& hold) const;
            bool operator()(
                const UKControllerPlugin::Dialog::DialogData& a, const UKControllerPlugin::Dialog::DialogData& b) const;

        } CompareDialogs;
    } // namespace Dialog
} // namespace UKControllerPlugin
