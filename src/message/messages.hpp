#pragma once

#include <string>

#include "utils/types.hpp"


namespace message
{

enum TransmissionId : u8
{
    Start = 0x10,
    Ack = 0x20,
    Nack
};

const std::string GetInfo("get_info");
const std::string Handshake("handshake");
const std::string ModuleInfo("module_info");

} // namespace message
