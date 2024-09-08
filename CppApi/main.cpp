#include "Communication/HttpServer.h"
#include "Utilities/Logger.h"

using namespace std;

int main()
{
    Utilities::Logger logger{Utilities::LogLevel::Debug};
    logger.Log(Utilities::LogLevel::Debug, "Starting server");
    auto httpServer{Communication::HttpsServer("127.0.0.1", 8080)};
    httpServer.Run();
    return 0;
}
