/*
 * Register_mmap.h
 *
 * Memory-mapped register access for Zynq/Kria FPGA via /dev/mem.
 * Thread-safe using epicsMutex.
 *
 * Based on Kria-Motor-Controller/src/sw/Reg.cpp
 */

#ifndef REGISTER_MMAP_H
#define REGISTER_MMAP_H

#include <cstdint>
#include <cstddef>
#include <sys/types.h>
#include <epicsMutex.h>

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

class Register {
public:
    Register(off_t baseAddr, size_t size);
    ~Register();

    Register(const Register&) = delete;
    Register& operator=(const Register&) = delete;

    /* Full 32-bit register read/write */
    uint32_t read(off_t offset);
    void write(off_t offset, uint32_t value);

    /* Bit-field read/write (read-modify-write for setField) */
    uint32_t getField(off_t offset, uint8_t bitPos, uint8_t width);
    void setField(off_t offset, uint8_t bitPos, uint8_t width, uint32_t value);

private:
    const off_t baseAddr_;
    const size_t size_;
    volatile uint32_t *mappedBase_;
    int fd_;
    epicsMutex mutex_;
};

#endif /* REGISTER_MMAP_H */
