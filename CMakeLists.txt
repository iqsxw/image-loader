cmake_minimum_required(VERSION 3.10)

project("image-loader" LANGUAGES CXX)

add_subdirectory(3rdparty/libjpeg-turbo)

target_compile_options(turbojpeg-static PUBLIC "-fPIC")

add_library(${PROJECT_NAME} SHARED
    image-loader.h
    image-loader.cpp)

target_link_libraries(${PROJECT_NAME}
    turbojpeg-static
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES PUBLIC_HEADER image-loader.h)

install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
