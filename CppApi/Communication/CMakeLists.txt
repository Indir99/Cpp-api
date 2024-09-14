# Set the source files in the current directory (folder with .h and .cpp files)
set(SOURCES
    HttpsSession.h
    HttpsSession.cpp
    HttpsListener.h
    HttpsListener.cpp
    HttpsServer.h
    HttpsServer.cpp
)

# Create a library or executable target for the source files
add_library(Communication ${SOURCES})
