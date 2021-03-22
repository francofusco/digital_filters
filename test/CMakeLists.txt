# Test utility functions
add_executable(test_utilities test_utilities.cpp)
# link GTest and pthread
target_link_libraries(test_utilities
  ${PROJECT_NAME}
  ${GTEST_LIBRARIES}
  pthread
)
# make the test runnable by ctest
gtest_discover_tests(test_utilities)

# Test the main "Filter" class
add_executable(test_filter test_filter.cpp)
# link GTest and pthread
target_link_libraries(test_filter
  ${PROJECT_NAME}
  ${GTEST_LIBRARIES}
  pthread
)
# make the test runnable by ctest
gtest_discover_tests(test_filter)
