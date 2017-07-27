#pragma once

#include <functional>
#include <memory>
#include <string>

#include "hal/net/http/asyncHttpResponse.hpp"
#include "logger/Logger.hpp"
#include "utils/types.hpp"

namespace net
{
namespace http
{

class AsyncHttpRequest
{
public:
    using SendCallback = std::function<void(u16 code, const std::string& type,
                                            const std::string& body)>;
    using SendResponseCallback = std::function<void(AsyncHttpResponse*)>;

    using ChunkedResponseParseCallback = std::function<std::size_t(
        u8*, std::size_t, std::size_t)>;

    using ChunkedResponseCallback = std::function<std::unique_ptr<AsyncHttpResponse>(const std::string& type,
                                                                                     ChunkedResponseParseCallback callback)>;

    using GetBodyCallback = std::function<std::string()>;
    AsyncHttpRequest()
        : logger_("AsyncHttpRequest"),
          sendCallback_(nullptr),
          sendResponseCallback_(nullptr),
          chunkedResponseCallback_(nullptr)
    {
    }

    ~AsyncHttpRequest()
    {
    }

    void setSendCallback(SendCallback sendCallback)
    {
        sendCallback_ = sendCallback;
    }

    void setSendResponseCallback(SendResponseCallback sendCallback)
    {
        sendResponseCallback_ = sendCallback;
    }

    void setChunkedResponseCallback(ChunkedResponseCallback chunkedCallback)
    {
        chunkedResponseCallback_ = chunkedCallback;
    }

    void setGetBodyCallback(GetBodyCallback getBodyCallback)
    {
        getBodyCallback_ = getBodyCallback;
    }

    void send(u16 code, const std::string& type, const std::string& msg)
    {
        if (nullptr == sendCallback_)
        {
            logger_.err() << "Callback for send hasn't been set for msg: " << msg << "\n";
            return;
        }
        sendCallback_(code, type, msg);
    }

    void send(AsyncHttpResponse* response)
    {
        if (nullptr == sendCallback_)
        {
            logger_.err() << "Callback for send response hasn't been set for msg: \n";
            return;
        }
        sendCallback_(response->getCode(), response->getType(), response->getMsg());
    }

    std::unique_ptr<AsyncHttpResponse> beginChunkedResponse(const std::string& type, ChunkedResponseParseCallback callback)
    {
        if (nullptr == chunkedResponseCallback_)
        {
            logger_.err() << "Callback for chunked response hasn't been set for msg: \n";
            return nullptr;
        }
        std::unique_ptr<AsyncHttpResponse> resp(std::move(chunkedResponseCallback_(type, callback)));
        return resp;
    }

    std::string getBody()
    {

        if (nullptr == getBodyCallback_)
        {
            logger_.err() << "Callback for getBody hasn't been set for msg: \n";
            return "";
        }
        return getBodyCallback_();
    }


private:
    logger::Logger logger_;
    std::string body_;
    SendCallback sendCallback_;
    SendResponseCallback sendResponseCallback_;
    ChunkedResponseCallback chunkedResponseCallback_;
    GetBodyCallback getBodyCallback_;
};

} // namespace http
} // namespace net
