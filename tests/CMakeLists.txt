# Download Unity if not already present
include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.6.0
    SOURCE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/vendor/unity
)
FetchContent_MakeAvailable(unity)

set(UNITY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/unity/src)
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

            target_link_libraries(${EXEC_NAME} PRIVATE core)

            # Lexer, parser, and evaluator tests are registered individually below
            if(NOT ${EXEC_NAME} STREQUAL "test_parser" AND
               NOT ${EXEC_NAME} STREQUAL "test_lexer" AND
               NOT ${EXEC_NAME} STREQUAL "test_evaluator" AND
               NOT ${EXEC_NAME} STREQUAL "test_environment")
                add_test(NAME ${EXEC_NAME} COMMAND ${EXEC_NAME})
            endif()
        endif()
    endif()
endforeach()

# Individual lexer tests — each runs as a separate CTest entry
set(LEXER_TESTS
    test_single_symbol
    test_symbols
    test_single_char_comparison_assignment
    test_double_char_comparison_operators
    test_delimiters
    test_identifiers
    test_numbers
    test_float_edge_cases
    test_keywords
    test_literals
    test_strings
    test_single_line_comment_only
    test_multiline_comment_only
    test_multiline_comment_between_code
    test_array
    test_negative_numbers
    test_nested_sexpr
    test_empty_list
    test_hyphenated_identifier
    test_quote_shorthand
    test_consecutive_expressions
    test_empty_string
    test_dots
    test_lambda_keyword
    test_nil_keyword
)

foreach(test_name ${LEXER_TESTS})
    add_test(NAME lexer/${test_name} COMMAND test_lexer ${test_name})
endforeach()

# Individual parser tests — each runs as a separate CTest entry
set(PARSER_TESTS
    # atoms
    test_empty_input
    test_int_expression
    test_float_expression
    test_string_expression
    test_bool_expression
    test_bool_false_expression
    test_nil_expression
    test_identifier_expression
    # arithmetic
    test_add
    test_subtract
    test_multiply
    test_divide
    test_modulo
    test_expression_statement_complex
    # comparison
    test_eq
    test_neq
    test_lt
    test_gt
    test_lte
    test_gte
    # boolean logic
    test_and
    test_or
    test_not
    # string
    test_string_concat
    # calls
    test_function_call
    test_nested_function_call
    test_nested_call
    # return
    test_return_statement
    test_return_with_call
    # let / set
    test_let_statement
    test_let_statement_with_float
    test_let_with_call
    test_let_anonymous_fn
    test_assignment_statement
    # multiple statements
    test_expression_statement
    test_multiple_statements
    test_multiple_let_statements
    # if
    test_if_statement
    test_if_else_statement
    test_if_with_complex_condition
    # do
    test_do_single
    test_do_multiple
    test_do_three
    test_do_in_if
    # fn
    test_function_statement
    test_function_statement_with_args
    test_fn_with_single_param
    test_anonymous_fn_no_params
    test_anonymous_fn_single_param
    test_anonymous_fn_two_params
    test_anonymous_fn_returning_fn
)

foreach(test_name ${PARSER_TESTS})
    add_test(NAME parser/${test_name} COMMAND test_parser ${test_name})
endforeach()

# Individual evaluator tests — each runs as a separate CTest entry
set(EVALUATOR_TESTS
    # atoms
    test_eval_integer
    test_eval_float
    test_eval_bool_true
    test_eval_bool_false
    test_eval_nil
    test_eval_string
    # arithmetic
    test_eval_add
    test_eval_subtract
    test_eval_multiply
    test_eval_divide
    test_eval_modulo
    test_eval_nested
    test_eval_add_single_int
    test_eval_add_single_float
    test_eval_add_int_int
    test_eval_add_float_int
    test_eval_add_float_float
    test_eval_subtract_single_int
    test_eval_subtract_single_float
    test_eval_subtract_int_int
    test_eval_subtract_float_int
    test_eval_subtract_float_float
    test_eval_multiply_single_int
    test_eval_multiply_single_float
    test_eval_multiply_int_int
    test_eval_multiply_float_int
    test_eval_multiply_float_float
    test_eval_divide_single_int
    test_eval_divide_single_float
    test_eval_divide_int_int
    test_eval_divide_float_int
    test_eval_divide_float_float
    # comparison
    test_eval_eq_true
    test_eval_eq_false
    test_eval_lt
    test_eval_gt
    # boolean logic
    test_eval_and
    test_eval_or
    test_eval_not
    # string
    test_eval_concat
    # let
    test_eval_let
    test_eval_let_expr
    # if
    test_eval_if_true
    test_eval_if_false
    test_eval_if_no_else
    # fn
    test_eval_fn_call
    test_eval_return
)

foreach(test_name ${EVALUATOR_TESTS})
    add_test(NAME evaluator/${test_name} COMMAND test_evaluator ${test_name})
endforeach()

# Individual environment tests — each runs as a separate CTest entry
set(ENVIRONMENT_TESTS
    test_env_new_is_empty
    test_env_get_missing
    test_env_set_and_get
    test_env_get_returns_same_pointer
    test_env_multiple_bindings
    test_env_overwrite_existing
    test_env_grows_past_initial_capacity
)

foreach(test_name ${ENVIRONMENT_TESTS})
    add_test(NAME environment/${test_name} COMMAND test_environment ${test_name})
endforeach()

# Example files — each is parsed by the kramer binary as a separate test
file(GLOB EXAMPLE_FILES "${PROJECT_SOURCE_DIR}/examples/*.krm")
foreach(example_path ${EXAMPLE_FILES})
    get_filename_component(example_name ${example_path} NAME_WE)
    add_test(NAME examples/${example_name} COMMAND kramer ${example_path})
endforeach()
