// Register access wrapper for FPGA device via /dev/vipic
// Compatible interface with zynq_reg.h for motorZynq
#pragma once

#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <sys/types.h>

class RegisterAccessException : public std::runtime_error {
public:
    explicit RegisterAccessException(const std::string& msg)
        : std::runtime_error(msg) {}
};

class Register {
public:
    // Constructor - opens /dev/vipic automatically
    // Parameters are for API compatibility with zynq_reg.h (mmap-based version)
    // They are ignored since /dev/vipic provides fixed register access
    // @param baseAddr: Ignored (for compatibility)
    // @param size: Ignored (for compatibility)
    // @throws RegisterAccessException if device cannot be opened
    Register(off_t baseAddr, size_t size);

    // Destructor - closes device
    ~Register();

    // Disable copy and move
    Register(const Register&) = delete;
    Register& operator=(const Register&) = delete;
    Register(Register&&) = delete;
    Register& operator=(Register&&) = delete;

    // Full 32-bit register read/write (offset in byte addresses for compatibility)
    uint32_t read(off_t offset);
    void write(off_t offset, uint32_t value);

    // Bit-field read/write (read-modify-write for setField)
    uint32_t getField(off_t offset, uint8_t bitPos, uint8_t width);
    void setField(off_t offset, uint8_t bitPos, uint8_t width, uint32_t value);

private:
    static constexpr const char* DEVICE_PATH = "/dev/vipic";
    int fd_;
};
