add_subdirectory(lib_json)
if(JSONCPP_WITH_TESTS)
    add_subdirectory(jsontestrunner)
    add_subdirectory(test_lib_json)
    add_subdirectory(test_lib_json_bos)
endif()
