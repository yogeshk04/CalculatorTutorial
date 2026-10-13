cmake_minimum_required(VERSION 3.16)
project(CalculatorTutorial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Phase 1: plain console app, no Qt needed yet ---
add_executable(CalculatorTutorial src/main.cpp)

# --- Starting from Phase 2 we will add Qt like this (kept here as a preview,
#     commented out, so it's easy to switch on when we get there): ---
# find_package(Qt6 REQUIRED COMPONENTS Widgets)
# qt_standard_project_setup()
# qt_add_executable(CalculatorTutorial src/main.cpp)
# target_link_libraries(CalculatorTutorial PRIVATE Qt6::Widgets)
