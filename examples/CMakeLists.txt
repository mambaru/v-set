add_subdirectory(compare)
add_subdirectory(multiset)
add_subdirectory(pointer_storage)
add_subdirectory(index_storage)

add_custom_target(
    examples DEPENDS
    compare_example
    multiset_example
    index_storage_example
    pointer_storage_example
)
