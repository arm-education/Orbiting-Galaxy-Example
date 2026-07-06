add_executable(users_solution_tests
    users_solution_tests.cpp
    ../src/users_solution/simulation.cpp
)

target_include_directories(users_solution_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/users_solution
)

target_link_libraries(users_solution_tests
    PRIVATE
        GTest::gtest_main
        m
)

include(GoogleTest)
gtest_discover_tests(users_solution_tests)
