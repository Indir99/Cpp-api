# Set the source files in the current directory (folder with .h and .cpp files)
set(SOURCES
    HttpServer.h
    HttpServer.cpp
)

# Create a library or executable target for the source files
add_library(Communication ${SOURCES})
