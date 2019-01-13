#pragma once
#include <gmock/gmock.h>

// Matches Two LPCTSTR's
MATCHER_P(LPCTSTREq, str, "")
{
    return lstrcmp(arg, str) == 0;
}


// Matches Two wstrings's
MATCHER_P(WstringEq, str, "")
{
    return arg == str;
}

// Matches Two RECTS
MATCHER_P(RectEq, rect, "")
{
    return arg.left == rect.left &&
        arg.top == rect.top &&
        arg.bottom == rect.bottom &&
        arg.right == rect.right;
}

MATCHER_P(PopupMenuItemEq, item, "")
{
    return arg.firstValue == item.firstValue &&
        arg.secondValue == item.secondValue &&
        arg.callbackFunctionId == item.callbackFunctionId &&
        arg.checked == item.checked &&
        arg.disabled == item.disabled &&
        arg.fixedPosition == item.fixedPosition;
}
