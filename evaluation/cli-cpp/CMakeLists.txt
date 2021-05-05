
# 
# External dependencies
# 

find_package(glm       REQUIRED)
find_package(cpplocate REQUIRED)
find_package(cppassist REQUIRED)

find_package(treemaphub-cpp-library   REQUIRED HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../../source-cpp")


# 
# Executable name and options
# 

# Target name
set(target treemaps-layouter)

# Exit here if required dependencies are not met
message(STATUS "Tool ${target}")


# 
# Sources
# 

set(sources
    common.cpp
    common.h
    export/export.h
    export/export.cpp
    export/export.inl
    
    main.cpp
)


# 
# Create executable
# 

# Build executable
add_executable(${target}
    MACOSX_BUNDLE
    ${sources}
)

# Create namespaced alias
add_executable(${META_PROJECT_NAME}::${target} ALIAS ${target})


# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${DEFAULT_INCLUDE_DIRECTORIES}

    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


#
# Libraries
#

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    cpplocate::cpplocate
    cppassist::cppassist
    glm::glm
    ${META_PROJECT_NAME}::measurements
    treemaphub-cpp-library::treemap
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}
)


# 
# Deployment
# 

# Executable
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT tools
    BUNDLE  DESTINATION ${INSTALL_BIN} COMPONENT tools
)
