add_executable(run_tests
        test_bytecode_encoder.cpp
)

target_link_libraries(run_tests PRIVATE
        BytecodeEncoder
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)