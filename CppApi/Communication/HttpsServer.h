#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>
#include "../Utilities/Logger.h"
#include "HttpsListener.h"

/* TODO:
 *  - Prepare Http Session
 *  - Prepare Http Listener
 *  - Prepare SSL context
 *  - Prepare multi threading
 *  - Prepare new io_context for every incoming session
*   - Prepare logger  */


// How it should work:
// When we start server we want to listen for incomming requests:
// When request arrive, we want to open a session and handle incomming request
// We need new thread for every session
// We simply want to handle a couple of requests at same time
// For now, we will keep it simple
// We will handle just one request at the time

namespace Communication {

class HttpsServer : public std::enable_shared_from_this<HttpsServer> {
public:
    //TODO: Implement server class that will manipulate with Listener and Sessions
    explicit HttpsServer(Utilities::Logger& logger,
                         std::string address,
                         unsigned short port);
    ~HttpsServer();
    HttpsServer(const HttpsServer& other) = delete;
    HttpsServer(HttpsServer&& other) = delete;
    HttpsServer& operator=(const HttpsServer& other) = delete;
    HttpsServer& operator=(HttpsServer&& other) = delete;
    void Run();

private:
    Utilities::Logger& m_logger;
    std::string m_address;
    unsigned short m_port;
    boost::asio::ssl::context m_sslContext;
    boost::asio::io_context m_ioContext;
    std::shared_ptr<HttpsListener> m_listener;
    std::vector<std::thread> m_threadPool;  // Thread pool to handle multiple connections

};

} // Communication
