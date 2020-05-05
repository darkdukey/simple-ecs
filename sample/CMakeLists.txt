file(GLOB SRC_FILES "*.h" "*.cpp" )

add_executable(program ${SRC_FILES})
target_link_libraries(program PRIVATE coco-core)
setWarnings(program)
setStandard(program)
