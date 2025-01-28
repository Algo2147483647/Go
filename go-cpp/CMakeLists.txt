cmake_minimum_required(VERSION 3.10)
project(Go)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Enable Qt's automatic MOC, UIC, and RCC processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set Qt path (note: this is typically done outside the script, e.g., via environment variable)
set(QT_PATH "C:\\Software\\Qt\\6.2.4\\msvc2019_64")
set(CMAKE_PREFIX_PATH "${QT_PATH}/lib/cmake")

file(GLOB PROJECT_SOURCES
     "${PROJECT_SOURCE_DIR}/core/*.cpp"
     "${PROJECT_SOURCE_DIR}/ai/*.cpp"
     "${PROJECT_SOURCE_DIR}/demo/*.cpp"
     "${PROJECT_SOURCE_DIR}/*.qrc" # 如果你有资源文件
)

# Find Qt packages
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Add executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Widgets)

