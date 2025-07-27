TARGET_	= nwy_open_app

CC 	    = arm-none-eabi-gcc.exe
LINK 	= arm-none-eabi-ld.exe
FROMELF	= arm-none-eabi-objcopy.exe

lds_dir = build

obj_dir = obj

lds_file = nwy_open_app.lds

TARGET = ${TARGET_}

ifndef proj_dir
    proj_dir = nwy_test_demo/nwy_test_cli
else
    proj_dir = nwy_test_demo/nwy_function_demo
endif

include nwy_test_demo/Makefile.mk
VPATH = $(addprefix $(test_dir)/, $(dir $(srcs)))

VPATH = $(addprefix $(proj_dir)/, $(dir $(srcs)))
inc_dir_full =-Iinclude
inc_dir_full +=-Iinclude/mbedtls
inc_dir_full +=-Iinclude/mbedtls/include
inc_dir_full += $(addprefix -I$(proj_dir)/, $(inc_dir))

#******************add your macro feature***************
feature += -DFEATURE_NWY_OPEN_APP -DFEATURE_NWY_ASR_PLAT

stdpath:= tools\GNU_Arm_Toolchain\arm-none-eabi\lib\thumb\v7\nofp
stdlibs:= $(stdpath)\libc.a \
		  $(stdpath)\libnosys.a \
		  tools\GNU_Arm_Toolchain\lib\gcc\arm-none-eabi\10.3.1\thumb\v7\nofp\libgcc.a

cflag:= $(inc_dir_full) $(feature) -c  -mcpu=cortex-R5 -mlittle-endian -mthumb -mthumb-interwork -mfloat-abi=soft -fno-builtin -Wall -ffunction-sections -fdata-sections -std=c99 -g -Os -fshort-wchar
LDFLAGS:= --gc-sections -T$(lds_dir)/$(lds_file) -nostdlib
copyflgs:= -R .note -R .comment -S -O binary

objs = $(patsubst %.c, ${obj_dir}/%.o, ${notdir ${srcs}})

${TARGET}.elf:$(objs)
	$(LINK) $(LDFLAGS) -Map ${TARGET}.map -o $@ --whole-archive $(objs) build/nwy_open_stub.a --no-whole-archive $(stdlibs)
	$(FROMELF) $(copyflgs) ${TARGET}.elf ${TARGET}.bin

#1. build srcs\*.c ---> $(obj_dir)/*.o
${obj_dir}/%.o:%.c
	if not exist $(obj_dir) mkdir $(obj_dir)
	$(CC) $(cflag) $< -o $@

clean:
	if exist ${TARGET}*.bin del ${TARGET}*.bin
	if exist ${TARGET}*.map del ${TARGET}*.map
	if exist ${TARGET}*.elf del ${TARGET}*.elf
	if exist buildlog.txt del buildlog.txt
	if exist $(obj_dir) rd /s /q  $(obj_dir)
