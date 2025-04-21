add_custom_target(test.logs ALL DEPENDS extract_logs)
add_custom_command(
    OUTPUT extract_logs
    DEPENDS test.elf
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src
    COMMENT "Extracting Logs"
    COMMAND ${CMAKE_OBJCOPY}
    ARGS --dump-section .logs=test.logs test.elf
)

add_custom_target(test.id ALL DEPENDS extract_build_id)
add_custom_command(
    OUTPUT extract_build_id
    DEPENDS test.elf
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src
    COMMENT "Extracting ID"
    COMMAND ${CMAKE_OBJCOPY}
    ARGS --dump-section .note.gnu.build-id=test.id test.elf
)

add_custom_target(test.art ALL DEPENDS logview_artifact)
add_custom_command(
    OUTPUT logview_artifact
    DEPENDS test.elf
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src
    COMMENT "Create Logview Artifact"
    COMMAND python
    ARGS ${CMAKE_SOURCE_DIR}/utl/merge.py test.id test.logs test.art
)

set_property(TARGET test.logs APPEND PROPERTY ADDITIONAL_CLEAN_FILES test.logs)
set_property(TARGET test.id APPEND PROPERTY ADDITIONAL_CLEAN_FILES test.id)
set_property(TARGET test.art APPEND PROPERTY ADDITIONAL_CLEAN_FILES test.art)