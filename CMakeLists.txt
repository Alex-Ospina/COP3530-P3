cmake_minimum_required(VERSION 3.26)
project(Project3)

set(CMAKE_CXX_STANDARD 17)
# The line below needs to be changed depending on where you have SFML on your local device
set(SFML_DIR "C:/Libraries/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
set(SFML_STATIC_LIBRARIES TRUE)

add_executable(Project3 main.cpp
        backendFunctionality.h
        backendFunctionality.cpp
)
target_link_libraries(Project3 sfml-graphics sfml-audio)
