cmake_minimum_required(VERSION 3.5)

project(Chess VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/lib)

set(SDL2_DIR ${PROJECT_SOURCE_DIR}/lib/cmake/SDL2)

set(SDL2_image_DIR ${PROJECT_SOURCE_DIR}/lib/cmake/SDL2_image)
set(SDL2_mixer_DIR ${PROJECT_SOURCE_DIR}/lib/cmake/SDL2_mixer)
set(SDL2_ttf_DIR ${PROJECT_SOURCE_DIR}/lib/cmake/SDL2_ttf)
set(SDL2_net_DIR ${PROJECT_SOURCE_DIR}/lib/cmake/SDL2_net)

find_package(SDL2 REQUIRED)

find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_net REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE PROJECT_HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_image SDL2_mixer SDL2_ttf SDL2_net)