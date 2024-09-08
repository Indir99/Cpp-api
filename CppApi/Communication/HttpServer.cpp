#include "HttpServer.h"

namespace Communication {

HttpsSession::HttpsSession(Utilities::Logger& logger,
                           boost::asio::ip::tcp::socket&& socket,
                           boost::asio::ssl::context& sslContext)
    : m_sslContext{sslContext}
    , m_stream{std::move(socket), sslContext}
    , m_logger{logger}
{}

HttpsSession::~HttpsSession(){}

void HttpsSession::Run(){

    m_logger.Log(Utilities::LogLevel::Debug, "HttpsSessionRun");

    // We need to be executing within a strand to perform async operations
    // on the I/O objects in this session. Although not strictly necessary
    // for single-threaded contexts, this example code is written to be
    // thread-safe by default.
    boost::asio::dispatch(m_stream.get_executor(),
                          boost::beast::bind_front_handler(&HttpsSession::Read,
                                                           shared_from_this()));
}

void HttpsSession::Read(){

    m_logger.Log(Utilities::LogLevel::Debug, "HttpsSessionRead");

    m_request = {};
    boost::beast::http::async_read(m_stream.next_layer(),
                                   m_buffer,
                                   m_request,
                                   boost::beast::bind_front_handler(&HttpsSession::OnRead,
                                                                    shared_from_this()));
}

void HttpsSession::OnRead(boost::beast::error_code ec,
                          std::size_t bytesTransferred) {

    boost::ignore_unused(bytesTransferred);
    if(ec) {
        // TODO: Look at what can go wrong in this case. If an error occurs, notify the sender
    }
    Write();
}

boost::beast::http::response<boost::beast::http::string_body> HttpsSession::HandleRequest() {

    m_logger.Log(Utilities::LogLevel::Debug, "HttpsSessionHandleRequest");

    namespace http = boost::beast::http;
    // Returns a bad request response
    auto const bad_request =
        [&](boost::beast::string_view why)
    {
        http::response<http::string_body> res{http::status::bad_request, m_request.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(m_request.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

    // Returns a not found response
    auto const not_found =
        [&](boost::beast::string_view target)
    {
        http::response<http::string_body> res{http::status::not_found, m_request.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(m_request.keep_alive());
        res.body() = "The resource '" + std::string(target) + "' was not found.";
        res.prepare_payload();
        return res;
    };

    // Returns a server error response
    auto const server_error =
        [&](boost::beast::string_view what)
    {
        http::response<http::string_body> res{http::status::internal_server_error, m_request.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(m_request.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    };

    // Make sure we can handle the method
    if( m_request.method() != http::verb::get &&
        m_request.method() != http::verb::head &&
        m_request.method() != http::verb::post)
        return bad_request("Unknown HTTP-method");

    // Request path must be absolute and not contain "..".
    if( m_request.target().empty() ||
        m_request.target()[0] != '/' ||
        m_request.target().find("..") != boost::beast::string_view::npos)
        return bad_request("Illegal request-target");

    // TODO: Respond to all types of requests


    // Respond to POST request
    http::response<http::string_body> res{http::status::ok, m_request.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(m_request.keep_alive());

    res.body() = "Dummy response";
    res.prepare_payload();
    return res;
}

void HttpsSession::Write(){

    m_logger.Log(Utilities::LogLevel::Debug, "HttpsSessionWrite");

    m_response = HandleRequest();
    // Write the response
    boost::beast::http::async_write(m_stream.next_layer(),
                                    m_response,
                                    boost::beast::bind_front_handler(&HttpsSession::OnWrite,
                                                                     shared_from_this()));
}

void HttpsSession::OnWrite(boost::beast::error_code ec,
                           std::size_t bytes_transferred) {

    boost::ignore_unused(bytes_transferred);

    if(ec) {
        // Error Handling
    }
    // TODO: Decide next step
    Read();
}

HttpsListener::HttpsListener(Utilities::Logger& logger,
                             std::string address,
                             unsigned short port,
                             boost::asio::io_context& ioContext,
                             boost::asio::ssl::context& sslContext)
    : m_ioContext{ioContext}
    , m_acceptor{m_ioContext}
    , m_sslContext{sslContext}
    , m_address{address}
    , m_port{port}
    , m_logger{logger}{
}

HttpsListener::~HttpsListener(){}

void HttpsListener::Run(){

    m_logger.Log(Utilities::LogLevel::Debug, "HttpsListenerRun");

    PrepareAcceptor();
    Accept();
}

void HttpsListener::PrepareAcceptor(){

    m_logger.Log(Utilities::LogLevel::Debug, "HttpsListenerPrepareAcceptor");

    boost::beast::error_code ec;
    boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::make_address(m_address), m_port};

    m_acceptor.open(endpoint.protocol(), ec);
    if(ec) {
        //Error handling
    }

    // Allow address reuse
    m_acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if(ec) {
        //Error handling
    }

    // Bind to the server address
    m_acceptor.bind(endpoint, ec);
    if(ec)
    {
        //Error handling
    }

    // Start listening for connections
    m_acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
    if(ec) {
        // Error handling
        return;
    }
}

void HttpsListener::Accept(){

    m_logger.Log(Utilities::LogLevel::Debug, "HttpsListenerAccept");

    m_acceptor.async_accept(m_ioContext,
                            boost::beast::bind_front_handler(&HttpsListener::OnAccept,
                                                             shared_from_this()));
}

void HttpsListener::OnAccept(boost::beast::error_code ec,
                             boost::asio::ip::tcp::socket stream){

    if(ec){
        //Error Handling
    } else {
        //Prepare Session
        // TODO: For every session we want a new thread
        auto session{std::make_shared<HttpsSession>(m_logger, std::move(stream), m_sslContext)};
        session->Run();
    }
    Accept();
}

HttpsServer::HttpsServer(Utilities::Logger& logger,
                         std::string address,
                         unsigned short port)
    : m_address{address}
    , m_port{port}
    , m_sslContext(boost::asio::ssl::context::tlsv13_server)
    , m_ioContext{}
    , m_listener{std::make_shared<HttpsListener>(logger, address, port, m_ioContext, m_sslContext)}
    , m_logger{logger} {
}

HttpsServer::~HttpsServer(){}

void HttpsServer::Run() {

    m_logger.Log(Utilities::LogLevel::Debug, "HttpsServerRun");

    m_listener->Run();
    // Create a thread pool
    const std::size_t threadCount = std::thread::hardware_concurrency(); // Or choose a fixed number
    for (std::size_t i = 0; i < threadCount; ++i) {
        m_threadPool.emplace_back([this]{
            m_ioContext.run();
        });
    }
    // Join all threads
    for (auto& t : m_threadPool) {
        t.join();
    }
}

} // Communication
