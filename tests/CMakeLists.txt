add_executable(Test1 test1.cpp)
target_include_directories(Test1 PUBLIC "${PROJECT_SOURCE_DIR}/include/jsonsimplecpp")
target_link_libraries(Test1 PUBLIC
                      jsonsimplecpp)

set_target_properties(Test1 PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_compile_definitions(Test1 PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

add_test(NAME Test1
         COMMAND Test1)