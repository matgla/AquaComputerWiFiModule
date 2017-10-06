#pragma once

#include <array>
#include <mutex>

#include <gsl/span>

#include "utils/types.hpp"

namespace container
{

template <std::size_t BUF_SIZE>
class Buffer
{
public:
    Buffer() : buffer_{}, writerIndex_{0}, readerIndex_{0}, size_{0}
    {
    }

    template <typename Type>
    u16 write(Type ch)
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        return writeUnsafe(ch);
    }

    template <typename Type>
    void write(gsl::span<const Type> str)
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        for (int i = 0; i < str.length(); ++i)
        {
            writeUnsafe(str[i]);
        }
    }

    u8 getByte()
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        return getByteUnsafe();
    }

    bool getValue(const u16 offset, u8& value)
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        if (offset >= size_)
        {
            return false;
        }

        if (size_)
        {
            int tempIndex = readerIndex_;
            if (tempIndex >= BUF_SIZE)
            {
                tempIndex = 0;
            }
            value = buffer_[tempIndex + offset];
            return true;
        }
        return false;
    }

    size_t getData(gsl::span<u8>& buf)
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        if (buf.length() < size_)
        {
            for (auto& byte : buf)
            {
                byte = getByteUnsafe();
            }
        }
        else
        {
            const size_t length = size_;
            for (size_t i = 0; i < length; i++)
            {
                buf[i] = getByteUnsafe();
            }
        }
        return buf.length();
    }

    u16 size()
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        return size_;
    }

    void flush()
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        writerIndex_ = 0;
        readerIndex_ = 0;
        size_ = 0;
    }

private:
    u8 getByteUnsafe()
    {
        if (size_)
        {
            if (readerIndex_ >= BUF_SIZE)
            {
                readerIndex_ = 0;
            }
            size_--;
            return buffer_[readerIndex_++];
        }
        return 0;
    }

    template <typename Type>
    u16 writeUnsafe(Type ch)
    {
        if (writerIndex_ >= BUF_SIZE)
        {
            writerIndex_ = 0;
        }
        incrementSize();
        buffer_[writerIndex_++] = ch;
        return writerIndex_;
    }

    void incrementSize()
    {
        if (size_ < BUF_SIZE)
        {
            ++size_;
        }
        else
        {
            ++readerIndex_;
        }
    }
    u8 buffer_[BUF_SIZE];
    u16 writerIndex_;
    u16 readerIndex_;
    u16 size_;
    std::mutex dataMutex_;
};

} // namespace container
