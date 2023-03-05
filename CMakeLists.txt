cmake_minimum_required(VERSION 3.0)
project( TerminalChat )

# find ncurses.h
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
# find boost asio
find_package(Boost 1.70.0 REQUIRED system)
include_directories(${BOOST_INCLUDE_DIR})

add_executable( Chat 
  ./src/Client/main.cpp
  ./src/Client/UI/utils.cpp
  ./src/Client/UI/ChatWindow.cpp
  ./src/Client/UI/DialoguesWindow.cpp
  ./src/Client/MessagingStructure.cpp
  ./src/Client/UI/ChatWindow.cpp
  ./src/Client/UI/InputWindow.cpp)

add_executable( Server
  ./src/Server/main.cpp)

target_link_libraries(Chat ${CURSES_LIBRARIES})
