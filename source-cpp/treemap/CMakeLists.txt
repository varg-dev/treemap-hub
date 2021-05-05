
# 
# External dependencies
# 

find_package(glm       REQUIRED)


# 
# Executable name and options
# 

# Target name
set(target treemap)

# Exit here if required dependencies are not met
message(STATUS "Lib ${target}")

# Set API export file and macro
string(MAKE_C_IDENTIFIER ${target} target_id)
string(TOUPPER ${target_id} target_id)
set(feature_file         "include/${target}/${target}_features.h")
set(export_file          "include/${target}/${target}_export.h")
set(template_export_file "include/${target}/${target}_api.h")
set(export_macro         "${target_id}_API")


# 
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include/${target}")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(headers
    ${include_path}/buffers/AbstractLinearizedBuffer.h
    ${include_path}/buffers/LinearizedBuffer.h
    ${include_path}/buffers/LinearizedBuffer.inl
    ${include_path}/linearizedtree/LinearizedTree.h
    ${include_path}/linearizedtree/LinearizedTree.inl
    ${include_path}/linearizedtree/LinearizedTreeNode.h
    ${include_path}/linearizedtree/LinearizedTreeNode.inl
    ${include_path}/loading/LinearizedCSVImporter.h
    ${include_path}/preprocessing/AttributeSumAggregation.h
    ${include_path}/preprocessing/TreeProcessing.h
    ${include_path}/layouting/WeightPartitioning.h
    ${include_path}/layouting/SingleLevelHilbertMoore.h
    ${include_path}/layouting/AbstractRow.h
    ${include_path}/layouting/DirectionalRow.h
    ${include_path}/layouting/HilbertMoorePattern.h
    ${include_path}/layouting/RectangularLayoutAlgorithm.h
    ${include_path}/layouting/Row.h
    ${include_path}/layouting/SingleLevelApproximation.h
    ${include_path}/layouting/SingleLevelPivot.h
    ${include_path}/layouting/SingleLevelSliceNDice.h
    ${include_path}/layouting/SingleLevelSliceNDiceByLevel.h
    ${include_path}/layouting/SingleLevelSpiral.h
    ${include_path}/layouting/SingleLevelSquarified.h
    ${include_path}/layouting/SingleLevelStrip.h
    ${include_path}/layouting/SingleLevelStripInverted.h
    ${include_path}/layouting/Slice.h
    ${include_path}/layouting/SpiralRow.h
    ${include_path}/layouting/TreemapLayouter.h
    ${include_path}/layout/Rect.h
    ${include_path}/layout/RectangularTreemapLayout.h
)

set(sources
    ${source_path}/buffers/AbstractLinearizedBuffer.cpp
    ${source_path}/linearizedtree/LinearizedTree.cpp
    ${source_path}/linearizedtree/LinearizedTreeNode.cpp
    ${source_path}/loading/LinearizedCSVImporter.cpp
    ${source_path}/preprocessing/AttributeSumAggregation.cpp
    ${source_path}/preprocessing/TreeProcessing.cpp
    ${source_path}/layouting/WeightPartitioning.cpp
    ${source_path}/layouting/SingleLevelHilbertMoore.cpp
    ${source_path}/layouting/AbstractRow.cpp
    ${source_path}/layouting/DirectionalRow.cpp
    ${source_path}/layouting/Row.cpp
    ${source_path}/layouting/SingleLevelApproximation.cpp
    ${source_path}/layouting/SingleLevelPivot.cpp
    ${source_path}/layouting/SingleLevelSliceNDice.cpp
    ${source_path}/layouting/SingleLevelSliceNDiceByLevel.cpp
    ${source_path}/layouting/SingleLevelSpiral.cpp
    ${source_path}/layouting/SingleLevelSquarified.cpp
    ${source_path}/layouting/SingleLevelStrip.cpp
    ${source_path}/layouting/SingleLevelStripInverted.cpp
    ${source_path}/layouting/Slice.cpp
    ${source_path}/layouting/SpiralRow.cpp
    ${source_path}/layouting/TreemapLayouter.cpp
    ${source_path}/layout/Rect.cpp
)


# 
# Create library
# 

# Build library
add_library(${target}
    ${sources}
    ${headers}
)

# Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})

# Export library for downstream projects
export(TARGETS ${target} NAMESPACE ${META_PROJECT_NAME}:: FILE ${PROJECT_BINARY_DIR}/cmake/${target}/${target}-export.cmake)

# Create API export header
generate_export_header(${target}
    EXPORT_FILE_NAME  ${export_file}
    EXPORT_MACRO_NAME ${export_macro}
)

generate_template_export_header(${target}
    ${target_id}
    ${template_export_file}
)


# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
    VERSION ${META_VERSION}
    SOVERSION ${META_VERSION_MAJOR}
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${PROJECT_BINARY_DIR}/source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include

    PUBLIC
    ${DEFAULT_INCLUDE_DIRECTORIES}

    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LIBRARIES}
    glm::glm

    INTERFACE
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE

    PUBLIC
    $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:${target_id}_STATIC_DEFINE>
    ${DEFAULT_COMPILE_DEFINITIONS}

    INTERFACE
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_COMPILE_OPTIONS}

    INTERFACE
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}

    INTERFACE
)


# 
# Deployment
# 

# Library
install(TARGETS ${target}
    EXPORT  "${target}-export"            COMPONENT dev
    RUNTIME DESTINATION ${INSTALL_BIN}    COMPONENT runtime
    LIBRARY DESTINATION ${INSTALL_SHARED} COMPONENT runtime
    ARCHIVE DESTINATION ${INSTALL_LIB}    COMPONENT dev
)

# Header files
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE}
    COMPONENT dev
)

# Generated header files
install(DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE}
    COMPONENT dev
)

# CMake config
install(EXPORT ${target}-export
    NAMESPACE   ${META_PROJECT_NAME}::
    DESTINATION ${INSTALL_CMAKE}/${target}
    COMPONENT   dev
)
