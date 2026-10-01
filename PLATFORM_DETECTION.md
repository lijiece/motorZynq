# Platform Detection and Base Address Configuration

## Overview

The motorZynq Makefile now automatically detects the Zynq platform and configures the appropriate PL (Programmable Logic) base address at compile time.

## Platform Detection

### Detection Method

The Makefile checks (in order):
1. `/proc/device-tree/compatible` for "zynqmp" or "zynq" strings
2. `uname -m` output: `aarch64` → ZynqMP, `armv7l` → Zynq-7000

### Platform-Specific Base Addresses

| Platform | Architecture | Base Address | Examples |
|----------|-------------|--------------|----------|
| **ZynqMP** (Zynq UltraScale+) | aarch64 (64-bit ARM) | `0x80000000` | KR260, KV260, ZCU102, ZCU104 |
| **Zynq-7000** | armv7l (32-bit ARM) | `0x43C00000` | MiniZed, Zybo, ZedBoard, Arty Z7 |

## Build Configuration

### Makefile Logic

```makefile
# Detect platform
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

# Pass to compiler for mmap-based builds
ifeq ($(VIPIC_EXISTS),no)
    USR_CXXFLAGS += -DPL_BASE_ADDR=$(PL_BASE_ADDR)
endif
```

### Build Output Example (KR260)

```
=====================================
Motor Zynq Build Configuration
=====================================
Platform:     ZynqMP
PL Base Addr: 0x80000000
/dev/vipic:   yes
Register:     Register.cpp (ioctl)
=====================================
```

## Code Usage

### zynq_reg.h (mmap-based)

The header defines a default that can be overridden:

```cpp
// Default PL base address (can be overridden by Makefile)
#ifndef PL_BASE_ADDR
    #if defined(__aarch64__)
        // ZynqMP (Zynq UltraScale+): 64-bit ARM
        #define PL_BASE_ADDR 0x80000000
    #else
        // Zynq-7000: 32-bit ARM
        #define PL_BASE_ADDR 0x43C00000
    #endif
#endif
```

This provides:
- **Makefile override**: `-DPL_BASE_ADDR=0x80000000` takes precedence
- **Fallback detection**: Uses `__aarch64__` compiler define
- **Safety**: Always has a valid default

### Runtime Override (EPICS IOC)

The base address can still be overridden at runtime:

```
# st.cmd
zynqMotorCreateController("MOTOR1", 4, 0x80000000, 10, 100)
#                                    ^^^^^^^^^^
#                                    Base address (can be different from default)
```

## Device Selection Logic

### Complete Decision Tree

```
                    /dev/vipic exists?
                   /                  \
                 YES                   NO
                  |                     |
          Register.cpp            zynq_reg.cpp
          (ioctl-based)           (mmap-based)
                |                     |
         Uses /dev/vipic        Uses /dev/mem
         (no base addr)      (needs base addr)
                                     |
                          Platform Detection
                         /                  \
                    ZynqMP              Zynq-7000
                       |                    |
                  0x80000000           0x43C00000
```

## Testing Different Configurations

### Test on ZynqMP with /dev/vipic (Current KR260)
```bash
cd /epics/modules/motor/modules/motorZynq/motorZynqApp/src
make clean
make
# Output: Platform: ZynqMP, Register: Register.cpp (ioctl)
```

### Test on ZynqMP without /dev/vipic (Simulated)
```bash
# Rename /dev/vipic temporarily
sudo mv /dev/vipic /dev/vipic.bak
make clean
make
# Output: Platform: ZynqMP, PL Base Addr: 0x80000000, Register: zynq_reg.cpp (mmap)
sudo mv /dev/vipic.bak /dev/vipic
```

### Test on Zynq-7000 (Different Hardware)
```bash
# On Zynq-7000 board (MiniZed, Zybo, etc.)
make clean
make
# Output: Platform: Zynq-7000, PL Base Addr: 0x43C00000, Register: zynq_reg.cpp (mmap)
```

## Manual Override

If the automatic detection is wrong, you can override:

### Override in Makefile
```makefile
# Before include $(TOP)/configure/RULES
USR_CXXFLAGS += -DPL_BASE_ADDR=0x70000000
```

### Override at EPICS IOC Shell
```
# Pass custom address as 3rd parameter
zynqMotorCreateController("MOTOR1", 4, 0x70000000, 10, 100)
```

## Benefits

1. **Portability**: Same source code works on Zynq-7000 and ZynqMP
2. **Automatic**: No manual configuration needed
3. **Fallback**: Multiple detection methods ensure it works
4. **Override**: Can be manually configured if needed
5. **Visibility**: Build output shows detected configuration

## Files Modified

- `Makefile` - Added platform detection and base address macro
- `zynq_reg.h` - Added default base address with fallback detection
- `zynqMotorDriver.h` - Conditional include based on device availability

## Backward Compatibility

- **Existing EPICS databases**: No changes needed
- **Runtime behavior**: Identical to before
- **API**: Unchanged
