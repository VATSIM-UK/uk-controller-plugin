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

// Matches Two Gdiplus Rects
MATCHER_P(GdiRectEq, rect, "")
{
    return rect.Equals(arg);
}

// Matches Two POINTS
MATCHER_P(PointEq, point, "")
{
    return arg.x == point.x &&
        arg.y == point.y;
}

// Matches Two POINTS
MATCHER_P(CPositionEq, position, "")
{
    return arg.m_Latitude == position.m_Latitude &&
        arg.m_Longitude == position.m_Longitude;
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
