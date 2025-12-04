# Add Unity
set(UNITY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/unity)
set(UNITY_SRC ${UNITY_DIR}/unity.c)

# List all test groups (subdirectories)
file(GLOB TEST_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")

foreach(testdir ${TEST_DIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${testdir})
        # Find all .c files in this test folder
        file(GLOB TEST_FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/${testdir}/*.c"
        )

        if(TEST_FILES)
            # Create test executable name ("lexer" → "test_lexer")
            string(REPLACE "/" "_" TEST_NAME "${testdir}")
            set(EXEC_NAME "test_${TEST_NAME}")

            add_executable(${EXEC_NAME}
                ${TEST_FILES}
                ${UNITY_SRC}
            )

            target_include_directories(${EXEC_NAME} PRIVATE
                ${PROJECT_SOURCE_DIR}/headers
                ${UNITY_DIR}
            )

            # Register with CTest
            add_test(NAME ${EXEC_NAME} COMMAND ${EXEC_NAME})
        endif()
    endif()
endforeach()
