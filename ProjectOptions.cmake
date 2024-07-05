macro(set_local_options)

    if(PROJECT_IS_TOP_LEVEL)
     include(cmake/StandardProjectSettings.cmake)
    endif()

    add_library(jsonsimplecpp_warnings INTERFACE)
    add_library(jsonsimplecpp_options INTERFACE)

    include(cmake/CompilerWarnings.cmake)
    set_project_warnings(
        jsonsimplecpp_warnings
        ""
        ""
        ""
        ""
        "")
endmacro()
