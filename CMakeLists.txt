add_definitions(-DFEATURE_NWY_OPEN_MQTT -DFEATURE_NWY) 
 
# Neoway CmakeList


set(SOURCE_TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_OBJECT_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/obj)

set(CMAKE_C_COMPILER_FORCED   1)
set(CMAKE_CXX_COMPILER_FORCED 1)

# set CROSS gcc
set(CROSS_COMPILE arm-none-eabi-)
include(cmake/toolchain-gcc.cmake)

# set ojbcopy 
set(CROSS_FROMELF ${CROSS_COMPILE}objcopy)

#message(STATUS "C FLAG:${CMAKE_C_FLAGS}")

# build target name
set(build_target nwy_open_app)

#set target project
project(${build_target} C CXX ASM)

set(BUILD_SHARED_LIBS OFF)

# default lib
set(core_stub_o ${CMAKE_CURRENT_SOURCE_DIR}/build/nwy_open_stub.a)

# Include directories for project
include_directories(
    include
    include/mbedtls
    include/mbedtls/include
    nwy_test_demo/nwy_test_cli/comm
    nwy_test_demo/nwy_test_cli/modules
)

#set_target_properties(${target} PROPERTIES OBJECT_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/obj)

# create open app image
add_executable(${build_target} ${ARGN} ${core_stub_o})

#add target source
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/comm/nwy_test_cli_utils.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/comm/nwy_test_cli_adpt.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/comm/nwy_test_cli_menu.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_os.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_driver.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_http.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_ftp.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_sntp.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_sim.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_sms.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_sdw.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_lbs.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_loc.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_iot.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_mbedtls_aes.c)
#target_sources(${build_targett} PRIVATE nwy_test_demo/nwy_test_cli/nwy_test_cli_mbedtls_ssltcp.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_network.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_data.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_tcpip.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_tcpip_ex.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_mbedtls_ssltcp.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_stk.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_fota.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/modules/nwy_test_cli_mbedtls_config.c)
# Custom Modules Import
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/CustomModules/LCD.c)
target_sources(${build_target} PRIVATE nwy_test_demo/nwy_test_cli/CustomModules/I2C.c)

# set target map file for target lin
set(target_map_file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${build_target}.map)

# link libraries for target
target_link_libraries(${build_target} PRIVATE 
    -Wl,--whole-archive ${CMAKE_CURRENT_SOURCE_DIR}/build/nwy_open_stub.a  -Wl,--no-whole-archive 
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/GNU_Arm_Toolchain/arm-none-eabi/lib/thumb/v7/nofp/libc.a
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/GNU_Arm_Toolchain/arm-none-eabi/lib/thumb/v7/nofp/libnosys.a
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/GNU_Arm_Toolchain/lib/gcc/arm-none-eabi/10.3.1/thumb/v7/nofp/libgcc.a
    -Wl,-Map=${target_map_file}
)

# set objcopy flags for complete stripping
set(COPY_FLAGS 
    -R .comment                # Remove comment section
    -R .note                   # Remove note section
    -S                         # Remove all symbols and relocation information
    -O binary                  # Output in raw binary format
)

# set the elf and bin file name
set(nwy_app_elf_file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${build_target}.elf)
set(nwy_app_bin_file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${build_target}.bin)

# Ensure path consistency across different operating systems.
file(TO_NATIVE_PATH "${nwy_app_elf_file}" nwy_app_elf_file_native)
file(TO_NATIVE_PATH "${nwy_app_bin_file}" nwy_app_bin_file_native)

# generate app.bin from app.elf
add_custom_command(
    TARGET ${build_target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Generating binary file: ${nwy_app_bin_file}"
    COMMAND ${CROSS_FROMELF} ${COPY_FLAGS} "${nwy_app_elf_file_native}" "${nwy_app_bin_file_native}"
    COMMAND ${CMAKE_COMMAND} -E echo "Binary file generation completed"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${nwy_app_bin_file}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/latest.bin"
    BYPRODUCTS "${nwy_app_bin_file}"
    VERBATIM
    COMMENT "Converting ELF to binary format"
)


