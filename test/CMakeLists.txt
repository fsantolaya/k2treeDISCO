set(BINARY ${CMAKE_PROJECT_NAME}_tst)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

## Copiado de https://github.com/google/googletest/blob/main/googletest/README.md ##
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/dd7a9d29a33de34836c345c3b753d4eba15c5f44.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(${BINARY} ${TEST_SOURCES})
target_link_libraries(${BINARY} gtest_main ${CMAKE_PROJECT_NAME}_lib)
add_test(NAME ${BINARY}_test COMMAND ${BINARY})