add_executable(test.elf startup.cpp main.cpp hal_gpio.cpp hal_uart.cpp)
target_link_libraries(test.elf PUBLIC cmsis f072)

target_include_directories(test.elf PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)

add_custom_target(test.bin ALL DEPENDS raw_binary)
add_custom_command(
    OUTPUT raw_binary 
    DEPENDS test.elf 
    COMMENT "Unpacking ELF"
    COMMAND ${CMAKE_OBJCOPY} 
    ARGS -O binary test.elf test.bin
)

target_link_options(test.elf PRIVATE -Xlinker --print-memory-usage)