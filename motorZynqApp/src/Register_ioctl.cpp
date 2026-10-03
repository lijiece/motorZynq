// Register access wrapper implementation using ioctl
// Compatible with zynq_reg.cpp interface for motorZynq
#include "Register_ioctl.hpp"
#include <linux/ioctl.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstring>
#include <cerrno>
#include <iomanip>
#include <iostream>

// IOCTL definitions (from pl_ioctl.h)
#define READ_REG  _IOWR('p', 8, pldrv_io_t *)
#define WRITE_REG _IOW('p', 9, pldrv_io_t *)   // Fixed: _IOW not _IOWR

struct pldrv_io_t {
    uint32_t address;
    uint32_t data;
};

Register::Register(off_t baseAddr, size_t size)
    : fd_(-1)
{
    // Parameters are ignored - /dev/vipic provides fixed register access
    (void)baseAddr;  // Suppress unused parameter warning
    (void)size;      // Suppress unused parameter warning

    fd_ = ::open(DEVICE_PATH, O_RDWR);
    if (fd_ < 0) {
        throw RegisterAccessException(
            std::string("Failed to open ") + DEVICE_PATH + ": " + std::strerror(errno));
    }
}

Register::~Register() {
    if (fd_ >= 0) {
        ::close(fd_);
    }
}

uint32_t Register::read(off_t offset) {
    pldrv_io_t io;
    // Convert byte offset to uint32_t units (divide by 4)
    io.address = static_cast<uint32_t>(offset / 4);
    io.data = 0;

    //std::cout << "Reading register 0x" << std::hex << std::setw(2) << std::setfill('0')
    //          << offset << std::dec << std::endl;
    if (::ioctl(fd_, READ_REG, &io) < 0) {
        throw RegisterAccessException(
            std::string("READ_REG failed at offset 0x") +
            std::to_string(offset) + ": " + std::strerror(errno));
    }

    return io.data;
}

void Register::write(off_t offset, uint32_t value) {
    pldrv_io_t io;
    // Convert byte offset to uint32_t units (divide by 4)
    io.address = static_cast<uint32_t>(offset / 4);
    io.data = value;

    if (::ioctl(fd_, WRITE_REG, &io) < 0) {
        throw RegisterAccessException(
            std::string("WRITE_REG failed at offset 0x") +
            std::to_string(offset) + ": " + std::strerror(errno));
    }
}

uint32_t Register::getField(off_t offset, uint8_t bitPos, uint8_t width) {
    if (width == 0 || width > 32 || bitPos + width > 32) {
        throw RegisterAccessException("Invalid bit field parameters");
    }

    uint32_t regValue = read(offset);

    // Create mask and extract field
    uint32_t mask = ((1u << width) - 1);
    return (regValue >> bitPos) & mask;
}

void Register::setField(off_t offset, uint8_t bitPos, uint8_t width, uint32_t value) {
    if (width == 0 || width > 32 || bitPos + width > 32) {
        throw RegisterAccessException("Invalid bit field parameters");
    }

    // Read-modify-write
    uint32_t regValue = read(offset);
    uint32_t mask = ((1u << width) - 1);
    regValue &= ~(mask << bitPos);           // Clear field
    regValue |= ((value & mask) << bitPos);  // Set new value
    write(offset, regValue);
}
