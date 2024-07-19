cmake_minimum_required(VERSION 3.26)

project(hello-asio-http-server)

find_package( Boost REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )
link_directories( ${Boost_LIBRARY_DIRS} )
set( Boost_USE_STATIC_LIBS        OFF )
set( Boost_USE_MULTITHREADED      ON )
set( Boost_USE_STATIC_RUNTIME     OFF )
set( BOOST_ALL_DYN_LINK           ON )

add_executable( ${PROJECT_NAME}
        connection.cpp
        connection_manager.cpp
        main.cpp
        mime_types.cpp
        reply.cpp
        request_handler.cpp
        request_parser.cpp
        server.cpp
)
target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} )
