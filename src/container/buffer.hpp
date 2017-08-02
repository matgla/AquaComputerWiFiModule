#pragma once

#include <array>
#include <mutex>

#include "utils/types.hpp"

namespace container
{

template <std::size_t BUF_SIZE>
class Buffer
{
  public:
    Buffer()
        : writerIndex_(0),
          readerIndex_(0),
          size_(0)
    {
    }

    template <typename Type>
    u16 write(Type ch)
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        return writeUnsafe(ch);
    }

    template <typename Type>
    void write(Type* str)
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        for (int i = 0; i < strlen(str); ++i)
        {
            writeUnsafe(str[i]);
        }
    }

    void write(u8* str, std::size_t len)
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        for (int i = 0; i < len; ++i)
        {
            writeUnsafe(str[i]);
        }
    }

    u8 getByte()
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        return getByteUnsafe();
    }

    bool getValue(u16 offset, u8& value)
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

    size_t getData(u8* buf, size_t len)
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        if (len < size_)
        {
            for (size_t i = 0; i < len; i++)
            {
                buf[i] = getByteUnsafe();
            }
        }
        else
        {
            size_t length = size_;
            for (size_t i = 0; i < length; i++)
            {
                buf[i] = getByteUnsafe();
            }
        }
        return len;
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
    volatile u8 buffer_[BUF_SIZE];
    volatile u16 writerIndex_;
    volatile u16 readerIndex_;
    volatile u16 size_;
    std::mutex dataMutex_;
};

}  // namespace container

