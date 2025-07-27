# Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.

# Configures CMake for using GCC

set(CMAKE_SYSTEM_NAME           Generic)
find_program(CMAKE_C_COMPILER   ${CROSS_COMPILE}gcc)
find_program(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)
find_program(CMAKE_READELF      ${CROSS_COMPILE}readelf)
find_program(CMAKE_LD           ${CROSS_COMPILE}ld)


set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)
set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
set(CMAKE_LINKER           ${CROSS_COMPILE}ld)
execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE gcc_version OUTPUT_STRIP_TRAILING_WHITESPACE)

# Feature flags
set(FEATURE_FLAGS
    "-DFEATURE_NWY_OPEN_APP"
    "-DFEATURE_NWY_ASR_PLAT"
)

# CPU specific options
set(CPU_FLAGS
    "-mcpu=cortex-R5"
    "-mlittle-endian"
    "-mthumb"
    "-mthumb-interwork"
    "-mfloat-abi=soft"
)

# Optimization and debug options
set(OPT_FLAGS
    "-Os"
    "-g"
	"-fshort-wchar"
)

# Code generation options
set(CODE_GEN_FLAGS
    "-fno-builtin"
    "-Wall"
    "-ffunction-sections"
    "-fdata-sections"
    "-std=c99"
)

# Linker options
set(LINKER_FLAGS
    "-Wl,--gc-sections"
    "-T${CMAKE_CURRENT_SOURCE_DIR}/build/nwy_open_app.lds"
    "-nostdlib"
)

# Combine all flags
string(JOIN " " COMBINED_FLAGS 
    ${FEATURE_FLAGS}
    ${CPU_FLAGS}
    ${OPT_FLAGS}
    ${CODE_GEN_FLAGS}
    ${LINKER_FLAGS}
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMBINED_FLAGS}")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFEATURE_NWY_OPEN_APP -DFEATURE_NWY_ASR_PLAT -mcpu=cortex-R5 -mlittle-endian -mthumb -mthumb-interwork -mfloat-abi=soft -fno-builtin -Wall -ffunction-sections -fdata-sections -std=c99 -g -Os -fshort-wchar -Wl,--gc-sections -T${CMAKE_CURRENT_SOURCE_DIR}/build/nwy_open_app.lds -nostdlib")

# C++ specific feature flags
set(CXX_FEATURE_FLAGS
    "-DFEATURE_NWY_OPEN_APP"
    "-DFEATURE_NWY_ASR_PLAT"
)

# C++ language options
set(CXX_LANG_FLAGS
    "-fno-exceptions"      # 禁用异常处理
    "-fno-rtti"           # 禁用运行时类型信息
    "-fno-threadsafe-statics"  # 禁用线程安全的静态局部变量初始化
)

# Combine C++ flags
string(JOIN " " COMBINED_CXX_FLAGS 
    ${CXX_LANG_FLAGS}
    ${CXX_FEATURE_FLAGS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMBINED_CXX_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti -fno-threadsafe-statics -DFEATURE_NWY_OPEN_APP -DFEATURE_NWY_ASR_PLAT")


# GNU ar will alreay create index
set(CMAKE_C_ARCHIVE_FINISH "")
set(CMAKE_CXX_ARCHIVE_FINISH "")

if(WITH_WERROR)
    add_compile_options(-Werror)
endif()

if(WITH_LINK_CREF)
    set(link_cref_option -Wl,-cref)
endif()

#set(multilib_opions ${abi_options})
#
#if((${gcc_version} VERSION_GREATER 8) AND (CONFIG_CPU_ARM_CA5))
#    set(multilib_opions -march=armv7-a+neon-vfpv4 -mthumb -mfloat-abi=hard)
#endif()
#
#execute_process(COMMAND ${CMAKE_C_COMPILER} ${multilib_opions} --print-file-name libgcc.a
#    OUTPUT_VARIABLE libgcc_file_name
#    OUTPUT_STRIP_TRAILING_WHITESPACE
#)