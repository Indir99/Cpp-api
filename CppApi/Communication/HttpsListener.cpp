#include "HttpsListener.h"
#include "HttpsSession.h"

namespace Communication {

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

} // Communication
