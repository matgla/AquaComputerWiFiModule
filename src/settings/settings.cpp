#include "settings/settings.hpp"

#include "hal/fs/file.hpp"

namespace settings
{

Settings& Settings::get()
{
    static Settings settings;
    return settings;
}

nlohmann::json& Settings::db()
{
    return Settings::get().getDb();
}

Settings::Settings()
{
    hal::fs::File settingsFile;
    settingsFile.open(SETTINGS_PATH);
    std::vector<u8> jsonData;
    jsonData.resize(settingsFile.size());
    settingsFile.read(reinterpret_cast<char*>(jsonData.data()), settingsFile.size());
    settingsFile.close();
    data_ = nlohmann::json::parse(jsonData);
}

nlohmann::json& Settings::getDb()
{
    return data_;
}

} // namespace settings
