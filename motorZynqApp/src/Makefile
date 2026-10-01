TOP = ../..
include $(TOP)/configure/CONFIG

# Build a support library
LIBRARY_IOC += motorZynq

# Detect platform and set PL base address
PLATFORM_CHECK := $(shell grep -q zynqmp /proc/device-tree/compatible 2>/dev/null && echo zynqmp || \
                         (grep -q zynq /proc/device-tree/compatible 2>/dev/null && echo zynq || \
                         (uname -m | grep -q aarch64 && echo zynqmp || echo zynq)))

ifeq ($(PLATFORM_CHECK),zynqmp)
    PL_BASE_ADDR := 0x80000000
    PLATFORM_NAME := ZynqMP
else
    PL_BASE_ADDR := 0x43C00000
    PLATFORM_NAME := Zynq-7000
endif

# Detect if /dev/vipic exists
VIPIC_EXISTS := $(shell test -e /dev/vipic && echo yes || echo no)

# Source files
ifeq ($(VIPIC_EXISTS),yes)
    motorZynq_SRCS += Register_ioctl.cpp
    USR_CXXFLAGS += -DUSE_VIPIC_REGISTER
    REG_TYPE := Register_ioctl (ioctl)
else
    motorZynq_SRCS += Register_mmap.cpp
    USR_CXXFLAGS += -DPL_BASE_ADDR=$(PL_BASE_ADDR)
    REG_TYPE := Register_mmap (mmap)
endif

motorZynq_SRCS += zynqMotorDriver.cpp
motorZynq_SRCS += zynqMotorRegister.cpp

DBD += motorZynqSupport.dbd
motorZynqSupport_DBD += motorZynq.dbd

motorZynq_LIBS += motor
motorZynq_LIBS += asyn
motorZynq_LIBS += $(EPICS_BASE_IOC_LIBS)

USR_CXXFLAGS += -std=c++14

include $(TOP)/configure/RULES

# Print once when entering O.* directory for install target
ifeq ($(T_A),linux-aarch64)
ifneq ($(filter install,$(MAKECMDGOALS)),)
ifndef ZYNQ_INFO_PRINTED
export ZYNQ_INFO_PRINTED := 1
$(info )
$(info ===== Motor Zynq: $(PLATFORM_NAME) | PL:$(PL_BASE_ADDR) | /dev/vipic:$(VIPIC_EXISTS) | $(REG_TYPE) =====)
$(info )
endif
endif
endif
