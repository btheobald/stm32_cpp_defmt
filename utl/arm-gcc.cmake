set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
set(TOOLCHAIN_PREFIX arm-none-eabi-)
find_program(BINUTILS_PATH ${TOOLCHAIN_PREFIX}gcc NO_CACHE)

if (NOT BINUTILS_PATH)
    message(FATAL_ERROR "ARM GCC toolchain not found")
endif ()

get_filename_component(ARM_TOOLCHAIN_DIR ${BINUTILS_PATH} DIRECTORY)
# Without that flag CMake is not able to pass test compilation check
if (${CMAKE_VERSION} VERSION_EQUAL "3.6.0" OR ${CMAKE_VERSION} VERSION_GREATER "3.6")
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
else ()
    set(CMAKE_EXE_LINKER_FLAGS_INIT "--specs=nosys.specs")
endif ()

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}gcc-ar)
set(CMAKE_RANLIB ${TOOLCHAIN_PREFIX}gcc-ranlib)

execute_process(COMMAND ${CMAKE_C_COMPILER} -print-sysroot
    OUTPUT_VARIABLE ARM_GCC_SYSROOT OUTPUT_STRIP_TRAILING_WHITESPACE)

set(COMPILER_FLAGS_INIT
    "-mcpu=${CHIP_CORE} \
    -Wall \
    -Werror \
    -mthumb \
    -mabi=aapcs \
    -Wl,--build-id \
    -fdata-sections \
    -ffunction-sections \
    -fomit-frame-pointer \
    -fno-exceptions -fno-rtti \
    -fdiagnostics-color=always \
    -fmacro-prefix-map=${CMAKE_SOURCE_DIR}=.")

set(COMPILER_FLAGS_INIT_RELEASE
    "-g0 \
     -O3 \
     ${COMPILER_FLAGS_INIT}")

set(COMPILER_FLAGS_INIT_DEBUG
    "-g3 \
     -Og \
     -DDEBUG \
     -gdwarf-3 \
     -pedantic \
     -save-temps \
     -gstrict-dwarf \
     ${COMPILER_FLAGS_INIT}")

set(CMAKE_C_FLAGS_DEBUG 
    "${COMPILER_FLAGS_INIT_DEBUG} -std=c23" 
    CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE 
    "${COMPILER_FLAGS_INIT_RELEASE} -std=c23" 
    CACHE STRING "" FORCE)

set(CMAKE_CXX_FLAGS_DEBUG 
    "${COMPILER_FLAGS_INIT_DEBUG} -std=c++20" 
    CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE
    "${COMPILER_FLAGS_INIT_RELEASE} -std=c++20" 
    CACHE STRING "" FORCE)

set(CMAKE_EXE_LINKER_FLAGS 
    "-Wl,--gc-sections \
     --specs=nano.specs \
     -mthumb \
     -mabi=aapcs \
     -Wl,-Map=${CMAKE_PROJECT_NAME}.map \
     -Wl,--script=${LINKER_SCRIPT}")

set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")

set(CMAKE_SYSROOT ${ARM_GCC_SYSROOT})
set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)