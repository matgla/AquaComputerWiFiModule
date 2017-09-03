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
    settingsFile.open(SETTINGS_PATH, "r");
    jsonData_.resize(settingsFile.size() + 1);
    settingsFile.read(jsonData_.data(), settingsFile.size());
    settingsFile.close();
    jsonData_.push_back(0);
    data_ = buffer_.parseObject(jsonData_.data());
}

JsonObject& Settings::getDb()
{
    return data_;
}

} // namespace settings
