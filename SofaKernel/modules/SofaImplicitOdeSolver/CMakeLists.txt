cmake_minimum_required(VERSION 3.12)
project(SofaImplicitOdeSolver LANGUAGES CXX)

set(SOFAIMPLICITEODESOLVER_SRC "src/${PROJECT_NAME}")

sofa_find_package(SofaFramework REQUIRED)

set(HEADER_FILES
    ${SOFAIMPLICITEODESOLVER_SRC}/config.h.in
    ${SOFAIMPLICITEODESOLVER_SRC}/initSofaImplicitOdeSolver.h
    ${SOFAIMPLICITEODESOLVER_SRC}/EulerImplicitSolver.h
    ${SOFAIMPLICITEODESOLVER_SRC}/StaticSolver.h
)

set(SOURCE_FILES
    ${SOFAIMPLICITEODESOLVER_SRC}/initSofaImplicitOdeSolver.cpp
    ${SOFAIMPLICITEODESOLVER_SRC}/EulerImplicitSolver.cpp
    ${SOFAIMPLICITEODESOLVER_SRC}/StaticSolver.cpp
)

sofa_find_package(SofaFramework REQUIRED) # SofaCore SofaSimulationCore

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC SofaCore SofaSimulationCore)

sofa_create_package_with_targets(
    PACKAGE_NAME ${PROJECT_NAME}
    PACKAGE_VERSION ${Sofa_VERSION}
    TARGETS ${PROJECT_NAME} AUTO_SET_TARGET_PROPERTIES
    INCLUDE_SOURCE_DIR "src"
    INCLUDE_INSTALL_DIR "${PROJECT_NAME}"
    RELOCATABLE "plugins"
)

# Tests
# If SOFA_BUILD_TESTS exists and is OFF, then these tests will be auto-disabled
cmake_dependent_option(SOFAIMPLICITEODESOLVER_BUILD_TESTS "Compile the automatic tests" ON "SOFA_BUILD_TESTS OR NOT DEFINED SOFA_BUILD_TESTS" OFF)
if(SOFAIMPLICITEODESOLVER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(${PROJECT_NAME}_test)
endif()
