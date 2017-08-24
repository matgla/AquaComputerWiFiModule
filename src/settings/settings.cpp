#include "settings/settings.hpp"

#include "hal/fs/file.hpp"

namespace settings
{

Settings& Settings::get()
{
    static Settings settings;
    return settings;
}

JsonObject& Settings::db()
{
    return Settings::get().getDb();
}

Settings::Settings()
{
    hal::fs::File settingsFile;
    settingsFile.open(SETTINGS_PATH);
    std::vector<char> jsonData;
    jsonData.resize(settingsFile.size());
    settingsFile.read(jsonData.data(), settingsFile.size());
    settingsFile.close();
    data_ = buffer_.parseObject(jsonData.data());
}

JsonObject& Settings::getDb()
{
    return data_;
}

} // namespace settings
