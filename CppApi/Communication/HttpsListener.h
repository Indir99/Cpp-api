#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>
#include "../Utilities/Logger.h"

namespace Communication {

class HttpsListener : public std::enable_shared_from_this<HttpsListener> {

public:
    explicit HttpsListener(Utilities::Logger& logger,
                           std::string address,
                           unsigned short port,
                           boost::asio::io_context& ioContext,
                           boost::asio::ssl::context& sslContext);
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
    Utilities::Logger& m_logger;
    std::string m_address;
    unsigned short m_port;
};

} // Communication
