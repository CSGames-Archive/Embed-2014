
# Path to top level ASF directory relative to this project directory.
PRJ_PATH = /opt/xdk-asf-3.13.1

# Target CPU architecture: ap, ucr1, ucr2 or ucr3
ARCH = ucr2

# Target part: none, ap7xxx or uc3xxxxx
PART = uc3a0512

# Target device flash memory details (used by the avr32program programming
# tool: [cfi|internal]@address
FLASH = internal@0x80000000

# Clock source to use when programming; xtal, extclk or int
PROG_CLOCK = int

# Application target name. Given with suffix .a for library and .elf for a
# standalone application.
TARGET = csgames_embed_avr32.elf

# List of C source files.
CSRCS = \
       avr32/boards/evk1100/led.c                         \
       avr32/boards/evk1100/init.c                        \
       avr32/drivers/adc/adc.c                            \
       avr32/drivers/flashc/flashc.c                      \
       avr32/drivers/gpio/gpio.c                          \
       avr32/drivers/intc/intc.c                          \
       avr32/drivers/tc/tc.c                              \
       avr32/drivers/pm/pm.c                              \
       avr32/drivers/pwm/pwm.c                              \
       avr32/drivers/pm/pm_conf_clocks.c                  \
       avr32/drivers/pm/power_clocks_lib.c                \
       avr32/drivers/usart/usart.c                        \
       avr32/drivers/spi/spi.c                        \
       avr32/components/display/dip204/dip204.c                        \
       avr32/utils/debug/debug.c                          \
       avr32/utils/debug/print_funcs.c                    \
       common/services/clock/uc3a0_a1/sysclk.c            \

# List of assembler source files.
ASSRCS = \
       avr32/drivers/intc/exception.S                     \
       avr32/utils/startup/startup_uc3.S                  \
       avr32/utils/startup/trampoline_uc3.S

# List of include paths.
INC_PATH = \
       avr32/boards                                       \
       avr32/boards/evk1100                               \
       avr32/boards/evk1100/board_config                  \
       avr32/components/joystick/skrhabe010               \
       avr32/components/display/dip204               \
       avr32/drivers/adc                                  \
       avr32/drivers/cpu/cycle_counter                    \
       avr32/drivers/flashc                               \
       avr32/drivers/gpio                                 \
       avr32/drivers/intc                                 \
       avr32/drivers/tc                                   \
       avr32/drivers/pm                                   \
       avr32/drivers/usart                                \
       avr32/drivers/spi                                \
       avr32/drivers/pwm                                \
       avr32/utils                                        \
       avr32/utils/debug                                  \
       avr32/utils/preprocessor                           \
       common/boards                                      \
       common/services/delay                              \
       common/services/clock                              \
       common/utils                                       \

# Additional search paths for libraries.
LIB_PATH = 

# List of libraries to use during linking.
LIBS = 

# Path relative to top level directory pointing to a linker script.
LINKER_SCRIPT = avr32/utils/linker_scripts/at32uc3a/0512/gcc/link_uc3a0512.lds

# Additional options for debugging. By default the common Makefile.in will
# add -g3.
DBGFLAGS = 

# Application optimization used during compilation and linking:
# -O0, -O1, -O2, -O3 or -Os
OPTIMIZATION = -Os

# Extra flags to use when archiving.
ARFLAGS = 

# Extra flags to use when assembling.
ASFLAGS = 

# Extra flags to use when compiling.
CFLAGS = 

# Extra flags to use when preprocessing.
#
# Preprocessor symbol definitions
#   To add a definition use the format "-D name[=definition]".
#   To cancel a definition use the format "-U name".
#
# The most relevant symbols to define for the preprocessor are:
#   BOARD      Target board in use, see boards/board.h for a list.
#   EXT_BOARD  Optional extension board in use, see boards/board.h for a list.
CPPFLAGS = \
       -D BOARD=EVK1100

# Extra flags to use when linking
LDFLAGS = \
       -nostartfiles -Wl,-e,_trampoline
