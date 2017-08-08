#pragma once

#include <gtest/gtest.h>

MATCHER_P2(ArrayCompare, a, b, "")
{
    bool areSame = std::equal(a, a + b, arg);
    if (!areSame)
    {
        *result_listener << std::hex;
        *result_listener << "\n\t Expected: [";
        for (int pos = 0; pos < b; pos++)
        {
            *result_listener << "0x" << static_cast<int>(a[pos]);
            if (pos < b - 1)
            {
                *result_listener << ", ";
            }
        }
        *result_listener << "],\n\t Received: [";
        for (int pos = 0; pos < b; pos++)
        {
            *result_listener << "0x" << static_cast<int>(arg[pos]);
            if (pos < b - 1)
            {
                *result_listener << ", ";
            }
        }
        *result_listener << "]";
    }

    return areSame;
}