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
#define WRITE_REG _IOW('p', 9, pldrv_io_t *)
#define READ_REG  _IOWR('p', 8, pldrv_io_t *)

struct pldrv_io_t {
    uint32_t address;
    uint32_t data;
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <offset> <value>" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Examples:" << std::endl;
        std::cerr << "  " << argv[0] << " 4  3   # Write 0x3 to DMA Control (0x10)" << std::endl;
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

        // Parse offset
        char* endptr;
        unsigned long offset = strtoul(argv[1], &endptr, 0) / 4;

        if (*endptr != '\0' || offset > 0xFFFF) {
            std::cerr << "Error: Invalid offset '" << argv[1] << "'" << std::endl;
            return -1;
        }

        unsigned int value = strtoul(argv[2], &endptr, 0);

        if (*endptr != '\0' ) {
            std::cerr << "Error: Invalid value '" << argv[2] << "'" << std::endl;
            return -1;
        }

        // Write register via ioctl
        pldrv_io_t io;
        io.address = static_cast<uint32_t>(offset);
        io.data = value;

        if (ioctl(fd, WRITE_REG, &io) < 0) {
            std::cerr << "Error: WRITE_REG failed for offset " << offset
                      << ": " << strerror(errno) << std::endl;
            return -1;
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


    close(fd);
    return 0;
}

