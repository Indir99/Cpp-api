#include <iostream>
#include "Communication/HttpServer.h"


using namespace std;

int main()
{
    std::cout<<"Starting server: "<<std::endl;
    auto httpServer{Communication::HttpsServer("127.0.0.1", 8080)};
    httpServer.Run();
    return 0;
}
