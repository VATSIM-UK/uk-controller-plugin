#include "pch/pch.h"
#include "dialog/CompareDialogs.h"
#include "dialog/DialogData.h"

using UKControllerPlugin::Dialog::DialogData;

namespace UKControllerPlugin {
    namespace Dialog {

        bool CompareDialogs::operator()(const DialogData & hold, unsigned int id) const
        {
            return hold.dialogId < id;
        }

        bool CompareDialogs::operator()(unsigned int id, const DialogData & hold) const
        {
            return id < hold.dialogId;
        }

        bool CompareDialogs::operator()(const DialogData & a, const DialogData & b) const
        {
            return a.dialogId < b.dialogId;
        }
    }  // namespace Dialog
}  // namespace UKControllerPlugin
