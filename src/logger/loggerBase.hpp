#pragma once

#include <memory>
#include <ostream>

namespace logger
{

class LoggerBase
{
public:
    LoggerBase();
    virtual ~LoggerBase() = default;

    template<typename T>
    LoggerBase& operator<<(const T& data)
    {
        (*stream_) << data;
      //  stream_->flush();
        return *this;
    }
protected:
    std::shared_ptr<std::ostream> stream_;
};

}  // namespace logger