#pragma once

#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ArduinoJson.h>

MATCHER_P(MessageIdCompare, a, "")
{
    if (!arg.containsKey("id"))
    {
        *result_listener << "Message hasn't message id";
        return false;
    }

    bool areSame = arg["id"] == a;
    if (!areSame)
    {
        *result_listener << "\n\t Expected: \"" << a << "\"\n\t Received: " << arg["id"];
    }

    return areSame;
}
