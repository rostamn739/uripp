set(CODENAME ${CMAKE_PROJECT_NAME})
seT(TEST_CODENAME ${CODENAME}_test)

set(SOURCES "main.cpp")
add_executable(${TEST_CODENAME} ${SOURCES})
add_test(NAME ${TEST_CODENAME} COMMAND ${TEST_CODENAME})

target_include_directories(${TEST_CODENAME} PUBLIC "${CMAKE_SOURCE_DIR}/include/")
target_include_directories(${TEST_CODENAME} PUBLIC "${CMAKE_SOURCE_DIR}/test/")
target_include_directories(${TEST_CODENAME} PUBLIC "${GTEST_ROOT}/googletest/include/")
target_link_libraries(${TEST_CODENAME} PUBLIC ${CODENAME} gtest)
