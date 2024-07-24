cmake_minimum_required(VERSION 3.5)

project(CppApi LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

# Adding subdirectories
add_subdirectory(Communication)
add_subdirectory(Utilities)

add_executable(CppApi main.cpp)

target_link_libraries(CppApi PRIVATE Communication Utilities OpenSSL::SSL)

include(GNUInstallDirs)
install(TARGETS CppApi
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
