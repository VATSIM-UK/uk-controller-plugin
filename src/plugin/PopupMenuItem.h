#pragma once

namespace UKControllerPlugin {
    namespace Plugin {

        /*
            Represents an item in a popup menu.
        */
        typedef struct PopupMenuItem {
            std::string firstValue = "";
            std::string secondValue = "";
            int callbackFunctionId = -1;
            int checked = 2;
            bool disabled = true;
            bool fixedPosition = false;

            /*
                Compares two PopupMenuItems for equality
            */
            bool operator==(const PopupMenuItem & compare) const
            {
                return this->firstValue == compare.firstValue &&
                    this->secondValue == compare.secondValue &&
                    this->callbackFunctionId == compare.callbackFunctionId &&
                    this->checked == compare.checked &&
                    this->disabled == compare.disabled &&
                    this->fixedPosition == compare.fixedPosition;
            }

        } PopupMenuItem;
    }  // namespace Plugin
}  // namespace UKControllerPlugin
