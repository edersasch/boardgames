set(CTEST_CUSTOM_POST_MEMCHECK
    "geninfo -o covall.info src; lcov -e covall.info '${CMAKE_CURRENT_SOURCE_DIR}/src/*' -o covsrc.info; genhtml -o Testing/coverage --no-function-coverage covsrc.info"
)
