#include "HttpsServer.h"

namespace Communication {

// TODO: Write doxygen comment
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

// TODO: Write doxygen comment
HttpsServer::~HttpsServer(){}

// TODO: Write doxygen comment
void HttpsServer::Run() {

    m_logger.Log(Utilities::LogLevel::Debug, "HttpsServer Run Method");

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
