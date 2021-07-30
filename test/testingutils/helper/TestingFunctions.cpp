#include "pch/pch.h"
#include "helper/TestingFunctions.h"

/*
    Returns true if two RECTs are equal
*/
bool RectsEqual(RECT left, RECT right)
{
    return left.left == right.left &&
        left.top == right.top &&
        left.right == right.right &&
        left.bottom == right.bottom;
}
