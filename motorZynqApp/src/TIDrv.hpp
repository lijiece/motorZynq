#pragma once

#include <array>
#include <cstdint>
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

uint32_t getDrvMWord( const DrvModel drvModel
                    , const uint32_t index
		    );

template<typename T>
uint32_t getDrvMVal ( const T& M
                    , uint32_t index
		    );

}

