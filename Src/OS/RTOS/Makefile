#/**************************************************************************
# * Kevin 2014-02-24                                                       *
# **************************************************************************/

#---------------------------------------------------------------------------
# Constant Variable definition
#---------------------------------------------------------------------------

ifeq ($(COMPILER_OS_CYGWIN),1)
TOPDIR=../../..
endif

include $(TOPDIR)/Tools/toolchain.def

CFLAGS		+= -DTLS_OS_FREERTOS=1
#---------------------------------------------------------------------------
# Target definition (User)
#---------------------------------------------------------------------------
GOAL  = $(LIB_DIR)/wmrtos.$(LIBTYPE)

#---------------------------------------------------------------------------
# Source section (User)
#---------------------------------------------------------------------------
ifeq ($(TOOL_GNU),1)
#ASM_SRC				+= ports/Other/portASM_gcc.s
ASM_SRC				+= 
else
ifeq ($(ARCH_M3), 0)
ASM_SRC				+= ports/Keil/portASM.s
else
ASM_SRC				+=
endif
endif

ifeq ($(ARCH_M3), 0)
C_SRC				+= ports/Other/port.c
endif
C_SRC				+= source/croutine.c
C_SRC				+= source/heap_2.c
C_SRC				+= source/heap_3.c
C_SRC				+= source/list.c
C_SRC				+= source/queue.c
C_SRC				+= source/timers.c
C_SRC				+= source/tasks.c
C_SRC				+= wm_osal_rtos.c
C_SRC				+= wm_os_extras.c
ifeq ($(ARCH_M3), 1)
ifeq ($(TOOL_GNU),1)
C_SRC				+= ports/port_m3_gcc.c
C_SRC				+= ports/port_wm_extras.c
else
C_SRC				+= ports/Keil/port_m3.c
endif
endif
#---------------------------------------------------------------------------
# Implicit rules
#---------------------------------------------------------------------------
.c.o:
	$(CC) $(CFLAGS) -c -o $*.o $< -I./include $(INCLUDES)

.s.o:
	$(ASM) $(ASMFLAGS) -c -o $*.o $< -I./include $(INCLUDES)

#---------------------------------------------------------------------------
# Explicit ruls
#---------------------------------------------------------------------------
OBJ_FILES	= $(C_SRC:%.c=%.o) $(ASM_SRC:%.s=%.o)

all: $(GOAL)

$(GOAL): $(OBJ_FILES)
	$(AR) $(ARFLAGS) $@ $(OBJ_FILES)

.PHONY: clean
clean:
	$(RM) -f $(GOAL)
	$(RM) -f $(OBJ_FILES:.o=.d) $(OBJ_FILES)
	
