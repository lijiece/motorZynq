#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

using namespace std::string_view_literals;

namespace TIDrv {

//----------------------------
// Driver model

enum DrvModel {
    DRV8434A,
    DRV8825,
    INVALID
};

DrvModel getDrvModel( const std::string& drvModelString );

//----------------------------
// M words

typedef struct
{
    uint32_t m_I;
    uint32_t m_T;
} DrvMBit;

const std::array<std::array<DrvMBit, 3>, 9> DRV8434A_M = {{
    {{ {0, 0}, {0, 0}, {0, 0} }},
    {{ {0, 0}, {0, 0}, {1, 0} }},    
    {{ {0, 0}, {0, 0}, {0, 1} }},
    {{ {0, 0}, {1, 0}, {0, 0} }},
    {{ {0, 0}, {1, 0}, {1, 0} }},
    {{ {0, 0}, {1, 0}, {0, 1} }},
    {{ {0, 0}, {1, 1}, {0, 0} }},
    {{ {0, 0}, {1, 1}, {0, 1} }},
    {{ {0, 0}, {1, 1}, {1, 0} }}
}};

const std::array<std::array<DrvMBit, 3>, 8> DRV8825_M = {{
    {{ {0, 0}, {0, 0}, {0, 0} }},
    {{ {0, 0}, {0, 0}, {1, 0} }},    
    {{ {0, 0}, {1, 0}, {0, 0} }},
    {{ {0, 0}, {1, 0}, {1, 0} }},
    {{ {1, 0}, {0, 0}, {0, 0} }},
    {{ {1, 0}, {0, 0}, {1, 0} }},
    {{ {1, 0}, {1, 0}, {0, 0} }},
    {{ {1, 0}, {1, 0}, {1, 0} }}
}};

/* The EPICS-facing value is the driver mode-table index. */
std::optional<uint32_t> getDrvMWord(DrvModel drvModel, uint32_t mode);
std::optional<uint32_t> getDrvUstepMode(DrvModel drvModel, uint32_t mWord);

}
