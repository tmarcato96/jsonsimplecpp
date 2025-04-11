add_executable(Test1 test1.cpp)
target_include_directories(Test1 PUBLIC "${PROJECT_SOURCE_DIR}/include/jsonsimplecpp")
target_link_libraries(Test1 PUBLIC
                      jsonsimplecpp)
add_test(NAME Test1
         COMMAND Test1)