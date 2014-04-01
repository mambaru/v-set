
add_subdirectory(compare)

add_custom_target(
    examples DEPENDS    
    compare_example
)