# ---- Variables ----

# # We use variables separate from what CTest uses, because those have customization issues set(COVERAGE_TRACE_COMMAND
# lcov -c -q -o "${PROJECT_BINARY_DIR}/coverage.info" -d "${PROJECT_BINARY_DIR}" --include "${PROJECT_SOURCE_DIR}/*"
# CACHE STRING "; separated command to generate a trace for the 'coverage' target")

# set(COVERAGE_HTML_COMMAND genhtml --legend -f -q "${PROJECT_BINARY_DIR}/coverage.info" -p "${PROJECT_SOURCE_DIR}" -o
# "${PROJECT_BINARY_DIR}/coverage_html" CACHE STRING "; separated command to generate an HTML report for the 'coverage'
# target")

# # ---- Coverage target ----

# add_custom_target( coverage COMMAND ${COVERAGE_TRACE_COMMAND} COMMAND ${COVERAGE_HTML_COMMAND} COMMENT "Generating
# coverage report" VERBATIM)

# ---- Add coverage flags

include(CMakeParseArguments)

function(target_add_coverage_flags target)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")

        cmake_parse_arguments(ARGUMENTS "PUBLIC;PRIVATE;INTERFACE" "" "" "${ARGV}")

        if(ARGUMENTS_PUBLIC)
            set(VISIBILITY PUBLIC)
        elseif(ARGUMENTS_PRIVATE)
            set(VISIBILITY PRIVATE)
        elseif(ARGUMENTS_INTERFACE)
            set(VISIBILITY INTERFACE)
        else()
            set(VISIBILITY PRIVATE)
        endif()

        target_compile_options(${target} ${VISIBILITY} --coverage -fno-inline)
        target_link_options(${target} ${VISIBILITY} --coverage)

        if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
            target_compile_options(${target} ${VISIBILITY} -fprofile-instr-generate -fcoverage-mapping)
        endif()
    else()
        message(WARNING "failed to add coverage information generation compiler is not support coverage")
    endif()
endfunction()

function(coverage_target target_name)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        find_program(LCOV lcov)
        find_program(GENHTML genhtml)

        set_target_properties(${target_name} PROPERTIES ENVIRONMENT "LLVM_PROFILE_FILE=${target_name}.profraw")

        add_custom_target(
            ${target_name}-lcov
            COMMAND ${LCOV} -d . --zerocounters
            COMMAND ${CMAKE_CTEST_COMMAND}
            COMMAND ${LCOV} -d . --capture -o coverage-${target_name}.info
            COMMAND ${LCOV} -r coverage-${target_name}.info '/usr/include/*' -o filtered-${target_name}.info
            WORKING_DIRECTORY $<TARGET_FILE_DIR:${target_name}>)

        add_custom_target(
            ${target_name}-genhtml
            COMMAND ${GENHTML} -o coverage filtered-${target_name}.info --legend
            DEPENDS ${target_name}-lcov
            WORKING_DIRECTORY $<TARGET_FILE_DIR:${target_name}>)

        add_custom_target(
            ${target_name}-coverage
            WORKING_DIRECTORY $<TARGET_FILE_DIR:${target_name}>
            DEPENDS ${target_name}-genhtml)

        add_custom_command(
            TARGET ${target_name}-coverage
            PRE_BUILD
            COMMAND find $<TARGET_FILE_DIR:${target_name}> -type f -name '*.gcda' -exec rm {} +)
    endif()
endfunction(coverage_target)
