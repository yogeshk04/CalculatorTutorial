cmake_minimum_required(VERSION 3.16)
project(CalculatorTutorial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Phase 2: first Qt window ---
# CMAKE_AUTOMOC lets CMake run Qt's "moc" tool automatically on any class
# that uses the Q_OBJECT macro (needed for signals & slots to work).
set(CMAKE_AUTOMOC ON)

# Tell CMake where to find Qt if it isn't already on CMAKE_PREFIX_PATH.
# Installed for this tutorial via: pip install aqtinstall, then
#   aqt install-qt windows desktop 6.9.3 win64_mingw --outputdir C:\Qt
if(NOT DEFINED CMAKE_PREFIX_PATH AND EXISTS "C:/Qt/6.9.3/mingw_64")
    list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.9.3/mingw_64")
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(CalculatorTutorial
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
)
target_link_libraries(CalculatorTutorial PRIVATE Qt6::Widgets)
