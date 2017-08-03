#include "hal/net/websocket.hpp"

#include <memory>
#include <string>
#include <thread>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

#include "logger/logger.hpp"

namespace http = boost::beast::http;           // from <beast/http.hpp>
namespace websocket = boost::beast::websocket; // from <beast/websocket.hpp>
namespace ip = boost::asio::ip;                // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;              // from <boost/asio.hpp>

namespace net
{
logger::Logger logger("WS");

class WebSocket::WebSocketWrapper
{
    using error_code = boost::system::error_code; // Saves typing
    using stream_type =
        websocket::stream<tcp::socket>; // The type of our websocket stream
    using clock_type =
        std::chrono::steady_clock; // For the timer

public:
    void start()
    {
        error_code ec;
        open(boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), port_}, ec);
    }

    class connection : public std::enable_shared_from_this<connection>
    {
        tcp::endpoint ep_; // The remote endpoing
        stream_type ws_;   // The websocket stream

        boost::asio::io_service::strand strand_; // Needed when threads > 1
        boost::beast::multi_buffer buffer_;      // Stores the current message
        boost::beast::drain_buffer drain_;       // Helps discard data on close
        std::size_t id_;                         // A small unique id

    public:
        /// Constructor
        connection(
            WebSocketWrapper& parent,
            tcp::endpoint const& ep,
            tcp::socket&& sock)
            : ep_(ep), ws_(std::move(sock)), strand_(ws_.get_io_service()), id_([] {
                  static std::atomic<std::size_t> n{0};
                  return ++n;
              }())
        {
            // Invoke the callback for new connections if set.
            // This allows the settings on the websocket stream
            // to be adjusted. For example to turn compression
            // on or off or adjust the read and write buffer sizes.
            //
            if (parent.mod_)
                parent.mod_(ws_);
        }

        // Called immediately after the connection is created.
        // We keep this separate from the constructor because
        // shared_from_this may not be called from constructors.
        void run()
        {
            // Read the websocket handshake and send the response
            ws_.async_accept_ex(
                [](websocket::response_type& res) {
                    res.insert(http::field::server, "websocket-server-async");
                },
                strand_.wrap(std::bind(
                    &connection::on_accept,
                    shared_from_this(),
                    std::placeholders::_1)));
        }

    private:
        // Called after the handshake is performed
        void on_accept(error_code ec)
        {
            if (ec)
                return fail("accept", ec);
            do_read();
        }

        // Read a message from the websocket stream
        void do_read()
        {
            // Read a message
            ws_.async_read(buffer_,
                           strand_.wrap(std::bind(
                               &connection::on_read,
                               shared_from_this(),
                               std::placeholders::_1)));
        }

        // Called after the message read completes
        void on_read(error_code ec)
        {
            // This error means the other side
            // closed the websocket stream.
            if (ec == websocket::error::closed)
                return;

            if (ec)
                return fail("read", ec);
            // Write the received message back
            std::stringstream ss;
            ss << boost::beast::buffers(buffer_.data());
            logger.info() << "Got data: " << ss.str();

            ws_.binary(ws_.got_binary());
            ws_.async_write(buffer_.data(),
                            strand_.wrap(std::bind(
                                &connection::on_write,
                                shared_from_this(),
                                std::placeholders::_1)));
        }

        // Called after the message write completes
        void on_write(error_code ec)
        {
            if (ec)
                return fail("write", ec);

            // Empty out the buffer. This is
            // needed if we want to do another read.
            //
            buffer_.consume(buffer_.size());

            do_read();
        }

        // Pretty-print an error to the log
        void fail(std::string what, error_code ec)
        {
            logger.err() << "WebSocket failed " << ec.message();
        }
    };

    WebSocketWrapper(const std::string& uri, u16 port)
        : uri_(uri), port_(port), sock_(ios_), acceptor_(ios_), work_(ios_)
    {
        thread_.reserve(1);

        thread_.emplace_back(
            [&] { ios_.run(); });
    }

    void fail(std::string what, error_code ec)
    {
        logger.err() << "duplo";
    }

    void do_accept()
    {
        acceptor_.async_accept(sock_, ep_,
                               std::bind(&WebSocketWrapper::on_accept, this,
                                         std::placeholders::_1));
    }

    // Called when receiving an incoming connection
    void on_accept(error_code ec)
    {
        // This can happen during exit
        if (!acceptor_.is_open())
            return;

        // This can happen during exit
        if (ec == boost::asio::error::operation_aborted)
            return;

        if (ec)
            fail("accept", ec);

        // Create the connection and run it
        std::make_shared<connection>(*this, ep_, std::move(sock_))->run();

        // Initiate another accept
        do_accept();
    }
    /// Destructor.
    ~WebSocketWrapper()
    {
        work_ = boost::none;
        ios_.dispatch([&] {
            error_code ec;
            acceptor_.close(ec);
        });
        for (auto& t : thread_)
            t.join();
    }

    /// Return the listening endpoint.
    tcp::endpoint
        local_endpoint() const
    {
        return acceptor_.local_endpoint();
    }

    /** Set a handler called for new streams.
    This function is called for each new stream.
    It is used to set options for every connection.
*/
    template <class F>
    void
        on_new_stream(F const& f)
    {
        mod_ = f;
    }

    /** Open a listening port.
    @param ep The address and port to bind to.
    @param ec Set to the error, if any occurred.
*/
    void
        open(tcp::endpoint const& ep, error_code& ec)
    {
        acceptor_.open(ep.protocol(), ec);
        if (ec)
            return fail("open", ec);
        acceptor_.set_option(
            boost::asio::socket_base::reuse_address{true});
        acceptor_.bind(ep, ec);
        if (ec)
            return fail("bind", ec);
        acceptor_.listen(
            boost::asio::socket_base::max_connections, ec);
        if (ec)
            return fail("listen", ec);
        do_accept();
    }

protected:
    std::ostream* log_;                       // Used for diagnostic output, may be null
    boost::asio::io_service ios_;             // The io_service, required
    tcp::socket sock_;                        // Holds accepted connections
    tcp::endpoint ep_;                        // The remote endpoint during accept
    std::vector<std::thread> thread_;         // Threads for the io_service
    boost::asio::ip::tcp::acceptor acceptor_; // The listening socket
    std::function<void(stream_type&)> mod_;   // Called on new stream
    boost::optional<
        boost::asio::io_service::work>
        work_; // Keeps io_service::run from returning
    std::string uri_;
    u16 port_;
};


// This helper will apply some settings to a WebSocket
// stream. The server applies it to all new connections.
//
class set_stream_options
{
    websocket::permessage_deflate pmd_;

public:
    set_stream_options(set_stream_options const&) = default;

    explicit set_stream_options(
        websocket::permessage_deflate const& pmd)
        : pmd_(pmd)
    {
    }

    template <class NextLayer>
    void
        operator()(websocket::stream<NextLayer>& ws) const
    {
        ws.set_option(pmd_);

        // Turn off the auto-fragment option.
        // This improves Autobahn performance.
        //
        ws.auto_fragment(false);

        // 64MB message size limit.
        // The high limit is needed for Autobahn.
        ws.read_message_max(64 * 1024 * 1024);
    }
};


WebSocket::WebSocket(const std::string& uri, u16 port)
    : webSocketWrapper_(new WebSocketWrapper(uri, port)),
      handler_([](std::unique_ptr<Message>) {})
{
    websocket::permessage_deflate pmd;
    pmd.client_enable = true;
    pmd.server_enable = true;
    pmd.compLevel = 3;
    webSocketWrapper_->on_new_stream(set_stream_options{pmd});
}

WebSocket::~WebSocket() = default;

void WebSocket::setHandler(HandlerCallback handler)
{
    handler_ = handler;
}

void WebSocket::start()
{
    webSocketWrapper_->start();
}

} // namespace net
