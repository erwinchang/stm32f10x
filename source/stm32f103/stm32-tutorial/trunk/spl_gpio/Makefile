PREFIX := arm-none-eabi-
CC := $(PREFIX)gcc
AS := $(PREFIX)as
AR := $(PREFIX)ar
LD := $(PREFIX)ld
OBJCOPY := $(PREFIX)objcopy

SED := sed
MV := mv
RM := rm

.SUFFIXES:
.SUFFIXES: .o .c
.SUFFIXES: .o .s

driver_src = driver/misc.c \
	     driver/stm32f10x_gpio.c \
	     driver/stm32f10x_rcc.c \
	     driver/stm32f10x_usart.c

startup_src := startup/startup_stm32f10x_hd.s
cmsis_src := cmsis/core_cm3.c \
	     cmsis/system_stm32f10x.c
usr_src := usr/main.c \
	   usr/stm32f10x_it.c

header_dir := include
ldscript := stm32f103vet6.ld
output_dir := build

driver := $(output_dir)/libspl.a
app_bin := $(output_dir)/gpio.bin
app_elf := $(output_dir)/gpio.out

CPPFLAGS := -I$(header_dir)
CFLAGS := -mcpu=cortex-m3 -mthumb
LDFLAGS := -T $(ldscript)

driver_objects := $(addprefix $(output_dir)/, $(driver_src:.c=.o))

app_objects := $(addprefix $(output_dir)/, $(startup_src:.s=.o))
app_objects += $(addprefix $(output_dir)/, $(cmsis_src:.c=.o))
app_objects += $(addprefix $(output_dir)/, $(usr_src:.c=.o))

dependencies := $(addprefix $(output_dir)/, $(driver_src:.c=.d))
dependencies += $(addprefix $(output_dir)/, $(cmsis_src:.c=.d))
dependencies += $(addprefix $(output_dir)/, $(usr_src:.c=.d))

define make-depend
  $(CC) $(CPPFLAGS) -MM $1 2> /dev/null| \
  $(SED) -e 's,\($(notdir $2)\) *:,$(dir $2)\1: ,' > $3.tmp
  $(SED) -e 's/#.*//' \
      -e 's/^[^:]*: *//' \
      -e 's/ *\\$$//' \
      -e '/^$$/ d' \
      -e 's/$$/ :/' $3.tmp >> $3.tmp
  $(MV) $3.tmp $3
endef

.PHONY: all clean

all: $(app_bin)

$(driver): $(driver_objects) 
	@echo Creating standard peripheral library...
	ar cr $@ $^

$(app_elf): $(app_objects) $(driver) $(ldscript)
	@echo Linking $@...
	$(LD) $(LDFLAGS) -o $@ $(app_objects) $(driver)

$(app_bin): $(app_elf)
	$(OBJCOPY) -O binary $(app_elf) $(app_bin)

clean:
	-@$(RM) -rf $(output_dir) 2> /dev/null

$(output_dir)/%.o : %.c
	@mkdir -p $(dir $@)
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(output_dir)/%.o : %.s
	@mkdir -p $(dir $@)
	$(AS) $(CFLAGS) $< -o $@

ifneq "$(MAKECMDGOALS)" "clean"
-include $(dependencies)
endif
