#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <cstdint>

extern "C" {
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
}

// IOCTL definitions
#define READ_REG  _IOWR('p', 8, pldrv_io_t *)

struct pldrv_io_t {
    uint32_t address;
    uint32_t data;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <offset> [offset2 ...]" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Examples:" << std::endl;
        std::cerr << "  " << argv[0] << " 0        # Read Device ID (0x00)" << std::endl;
        std::cerr << "  " << argv[0] << " 1        # Read Version (0x04)" << std::endl;
        std::cerr << "  " << argv[0] << " 4        # Read DMA Control (0x10)" << std::endl;
        std::cerr << "  " << argv[0] << " 0 1 2 3  # Read multiple registers" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Note: Offset is in uint32_t units (4-byte aligned)" << std::endl;
        std::cerr << "      Offset 1 = byte address 0x04" << std::endl;
        return 1;
    }

    // Open device
    int fd = open("/dev/vipic", O_RDWR);
    if (fd < 0) {
        std::cerr << "Error: Failed to open /dev/vipic: " << strerror(errno) << std::endl;
        std::cerr << "Make sure:" << std::endl;
        std::cerr << "  - Driver is loaded: lsmod | grep pldrv" << std::endl;
        std::cerr << "  - Device exists: ls -la /dev/vipic" << std::endl;
        return 1;
    }

    // Read each requested register
    for (int i = 1; i < argc; i++) {
        // Parse offset
        char* endptr;
        unsigned long offset = strtoul(argv[i], &endptr, 0) / 4;

        if (*endptr != '\0' || offset > 0xFFFF) {
            std::cerr << "Error: Invalid offset '" << argv[i] << "'" << std::endl;
            continue;
        }

        // Read register via ioctl
        pldrv_io_t io;
        io.address = static_cast<uint32_t>(offset);
        io.data = 0;

        if (ioctl(fd, READ_REG, &io) < 0) {
            std::cerr << "Error: READ_REG failed for offset " << offset
                      << ": " << strerror(errno) << std::endl;
            continue;
        }

        // Display result
        if (argc > 2) {
            std::cout << "Register[" << offset << "]";
        } else {
            std::cout << "Register[" << offset << "]";
        }
        
        std::cout << "(0x" << std::hex << std::setw(2) << std::setfill('0') 
                  << (offset) << std::dec << "): ";
        std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') 
                  << io.data << std::dec << std::endl;

    }

    close(fd);
    return 0;
}

