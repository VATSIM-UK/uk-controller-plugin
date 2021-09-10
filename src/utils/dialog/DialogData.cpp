#include "DialogData.h"

namespace UKControllerPlugin::Dialog {
    auto DialogData::operator==(const DialogData& compare) const -> bool
    {
        return this->dialogId == compare.dialogId;
    }
} // namespace UKControllerPlugin::Dialog
