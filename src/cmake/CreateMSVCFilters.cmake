# used for creating MSVC filters that resample folter structures
function(source_group_special _SOURCES _SRC_DIR)
    foreach(_source IN ITEMS ${_SOURCES})
        get_filename_component(_source_path "${_source}" PATH)
        file(RELATIVE_PATH _source_path_rel "${_SRC_DIR}" "${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
        source_group("${ARGV2}${_group_path}" FILES "${_source}")
    endforeach()
endfunction()