#pragma once

#include <string>

#include <json.hpp>

#ifdef X86_ARCH
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif // X86_ARCH

#if defined(X86_ARCH) && defined(_WIN32)
const std::string
    SETTINGS_PATH(fs::path(__FILE__).parent_path().parent_path().parent_path().string() +
                  "\\settings_windows.json");
#elif defined(ESP8266_ARCH)
const std::string SETTINGS_PATH("/settings.json");
#endif // X86_ARCH && WIN32

namespace settings
{

class Settings
{
public:
    static Settings& get();
    static nlohmann::json& db();

private:
    Settings();
    Settings(Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings(const Settings&) = delete;
    Settings(const Settings&&) = delete;
    Settings& operator=(Settings&) = delete;

    nlohmann::json& getDb();

    nlohmann::json data_;
};

} // namespace settings
