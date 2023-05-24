cmake_minimum_required(VERSION 3.20)
project(zenohcxx_examples LANGUAGES C CXX)

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif()


#include_directories("/usr/local/include/")
#include_directories("/usr/local/include/zenoh-pico")
#include_directories("/usr/local/include/zenohcxx")
#include_directories("${CMAKE_CURRENT_BINARY_DIR}")

#link_directories("/usr/local/lib")
#set(PROJECT_LINK_LIBS PUBLIC zenohcxx::zenohpico)


#2. Use find_package (if corresponding library installed):

    find_package(zenohpico REQUIRED PATHS "/usr/local/")
    find_package(zenohc REQUIRED PATHS "/usr/local/")
    find_package(zenohcxx REQUIRED PATHS "/usr/local/")

#add_executable(zp_simple z_simple.cxx)
#target_link_libraries(zp_simple PRIVATE zenohcxx::zenohpico) #${PROJECT_LINK_LIBS})
#set_property(TARGET zp_simple PROPERTY LANGUAGE CXX)
#set_property(TARGET zp_simple PROPERTY CXX_STANDARD 17)

add_executable(ishape_subscriber ishape_subscriber.cxx)
target_link_libraries(ishape_subscriber PRIVATE zenohcxx::zenohc::lib jsoncpp)
set_property(TARGET ishape_subscriber PROPERTY LANGUAGE CXX)
set_property(TARGET ishape_subscriber PROPERTY CXX_STANDARD 17)


