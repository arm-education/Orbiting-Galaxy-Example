cmake_minimum_required(VERSION 3.16)
project(spiraling-galaxies LANGUAGES CXX)

include(FetchContent)
include(CTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -O2: optimise both binaries identically.
# -g:  retain debug info so Performix source view can resolve line numbers.
# The only variable between the two binaries is data layout, not compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")

if(BUILD_TESTING)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(test)
endif()


add_executable(baseline
    src/baseline/baseline.cpp
    src/baseline/simulation.cpp
)

add_executable(optimized
    src/optimized/optimized.cpp
    src/optimized/simulation.cpp
)

add_executable(users_solution
    src/users_solution/main.cpp
    src/users_solution/simulation.cpp
)

target_link_libraries(baseline  m)
target_link_libraries(optimized m)
target_link_libraries(users_solution m)
