cmake_minimum_required(VERSION 3.15)
project(stoatBot CXX) # change name here (not needed here)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static") im using this lator to try to make it work without needing to be compiled

find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(test # change name here
    ../run.cpp
    ../networking/websocket.cpp
    ../networking/heartbeat.cpp
    ../events/main.cpp
    ../events/messageEvents.cpp
    ../requests/main.cpp
    ../requests/requestCalls.cpp
    ../parsing/main.cpp
    # your files under here
    ../../example.cpp # uh eventually remove this so the user can combile like multiple oness
)

target_link_libraries(test # change name here
    PRIVATE
        Boost::system
        OpenSSL::SSL
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
)

find_package(Threads REQUIRED)
target_link_libraries(test # change name here
    PRIVATE Threads::Threads
)
