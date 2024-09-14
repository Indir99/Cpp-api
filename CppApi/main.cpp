#include "Communication/HttpsServer.h"
#include "Utilities/Logger.h"

using namespace std;

/* TODO List:
 * 1. Provide multithreading (for each session we need new thread - with new socket)
 * 2. After exchanging messages close socket and thread
 * 3. Check for memory leaks with valgrind tool
 * 4. Provide support for json and soap messages
 */
// consts
const std::string address{"127.0.0.1"};
const unsigned short port{8080};

int main()
{
    Utilities::Logger logger{Utilities::LogLevel::Debug};
    logger.Log(Utilities::LogLevel::Debug, "Starting HTTP/HTTPS server");
    auto httpServer{Communication::HttpsServer(logger, address, port)};
    httpServer.Run();
    return 0;
}
