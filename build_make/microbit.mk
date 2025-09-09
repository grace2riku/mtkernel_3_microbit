################################################################################
# micro T-Kernel 3.00.05  makefile
################################################################################

GCC := arm-none-eabi-gcc
AS := arm-none-eabi-gcc
LINK := arm-none-eabi-gcc

CFLAGS := -mcpu=cortex-m4 -mthumb -ffreestanding \
    -std=gnu11 \
    -O0 -g3 \
    -MMD -MP \
    -mfpu=fpv4-sp-d16 -mfloat-abi=hard

ASFLAGS := -mcpu=cortex-m4 -mthumb -ffreestanding \
    -x assembler-with-cpp \
    -O0 -g3 \
    -MMD -MP \
    -mfpu=fpv4-sp-d16 -mfloat-abi=hard

LFLAGS := -mcpu=cortex-m4 -mthumb -ffreestanding \
    -nostartfiles \
    -O0 -g3 \
    -mfpu=fpv4-sp-d16 -mfloat-abi=hard

# libm をリンク
LIBS    := -lm

LNKFILE := "../etc/linker/microbit/tkernel_map.ld"

include mtkernel_3/lib/libtm/sysdepend/microbit/subdir.mk
include mtkernel_3/lib/libtm/sysdepend/no_device/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/nrf5/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/core/armv7m/subdir.mk
include mtkernel_3/kernel/sysdepend/microbit/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/nrf5/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/core/armv7m/subdir.mk
