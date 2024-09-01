#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>


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

class HttpsSession : public std::enable_shared_from_this<HttpsSession> {

public:
    explicit HttpsSession(boost::asio::ip::tcp::socket&& socket,
                          boost::asio::ssl::context& sslContext);
    ~HttpsSession();
    HttpsSession(const HttpsSession& other) = delete;
    HttpsSession(HttpsSession&& other) = delete;
    HttpsSession& operator=(const HttpsSession& other) = delete;
    HttpsSession& operator=(HttpsSession&& other) = delete;

    /**
     * @brief Run
     */
    void Run();

private:

    /**
     * @brief Read
     */
    void Read();

    /**
     * @brief OnRead
     * @param ec
     * @param bytesTransferred
     */
    void OnRead(boost::beast::error_code ec,
                std::size_t bytesTransferred);

    /**
     * @brief HandleRequest
     * @return
     */
    boost::beast::http::response<boost::beast::http::string_body> HandleRequest();

    /**
     * @brief Write
     */
    void Write();

    /**
     * @brief OnWrite
     * @param ec
     * @param bytes_transferred
     */
    void OnWrite(boost::beast::error_code ec,
                 std::size_t bytes_transferred);

    void CloseSocket();

    boost::beast::flat_buffer m_buffer;
    boost::beast::http::request<boost::beast::http::string_body> m_request;
    boost::beast::http::response<boost::beast::http::string_body> m_response;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> m_stream;
    boost::asio::ssl::context& m_sslContext;
};

class HttpsListener : public std::enable_shared_from_this<HttpsListener> {

public:
    explicit HttpsListener(std::string address, unsigned short port, boost::asio::io_context& ioContext, boost::asio::ssl::context& sslContext);
    ~HttpsListener();
    HttpsListener(const HttpsListener& other) = delete;
    HttpsListener(HttpsListener&& other) = delete;
    HttpsListener& operator=(const HttpsListener& other) = delete;
    HttpsListener& operator=(HttpsListener&& other) = delete;

    void Run();

private:
    void PrepareAcceptor();
    void Accept();
    void OnAccept(boost::beast::error_code ec,
                  boost::asio::ip::tcp::socket stream);

    boost::asio::io_context& m_ioContext;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ssl::context& m_sslContext;
    std::string m_address;
    unsigned short m_port;
};


class HttpsServer : public std::enable_shared_from_this<HttpsServer> {
public:
    //TODO: Implement server class that will manipulate with Listener and Sessions
    explicit HttpsServer(std::string address, unsigned short port);
    ~HttpsServer();
    HttpsServer(const HttpsServer& other) = delete;
    HttpsServer(HttpsServer&& other) = delete;
    HttpsServer& operator=(const HttpsServer& other) = delete;
    HttpsServer& operator=(HttpsServer&& other) = delete;
    void Run();

private:
    std::string m_address;
    unsigned short m_port;
    boost::asio::ssl::context m_sslContext;
    boost::asio::io_context m_ioContext;
    std::shared_ptr<HttpsListener> m_listener;
    std::vector<std::thread> m_threadPool;  // Thread pool to handle multiple connections

};


} // Communication
