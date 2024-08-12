add_library(jsonsimplecppLib preproc_stream.cpp
                             tokenizer.cpp
                             tokens.cpp
                             parser.cpp
                             node.cpp)       

target_include_directories(jsonsimplecppLib PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(jsonsimplecppLib PUBLIC jsonsimplecpp_warnings
                                              jsonsimplecpp_options)       

add_executable(main main.cpp)

target_link_libraries(main PUBLIC jsonsimplecppLib
                                  jsonsimplecpp_warnings
                                  jsonsimplecpp_options)           