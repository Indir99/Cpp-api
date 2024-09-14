#include "Communication/HttpsServer.h"
#include "Utilities/Logger.h"

using namespace std;

// consts
const std::string address{"127.0.0.1"};
const unsigned short port{8080};

int main()
{
    Utilities::Logger logger{Utilities::LogLevel::Debug};
    logger.Log(Utilities::LogLevel::Debug, "Starting server");
    auto httpServer{Communication::HttpsServer(logger, address, port)};
    httpServer.Run();
    return 0;
}
