#pragma once

namespace database
{

class Database
{
public:
    static Database& get();
private:
    Database() = default;
    
    Database(Database&) = delete;
    Database(Database&&) = delete;
    Database(const Database&) = delete;
    Database(const Database&&) = delete;
};  

} // namespace database


