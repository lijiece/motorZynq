#include <TIDrv.hpp>

#include <array>
#include <cstdint>
#include <cstdlib>
#include <string_view>

using namespace std::string_view_literals;

namespace TIDrv {

DrvModel getDrvModel( const std::string& drvModelString )
{
    if (   drvModelString == "DRV8434A"sv
       ||  drvModelString == "drv8434a"sv
       )
        return DrvModel::DRV8434A;
    else if (   drvModelString == "DRV8825"sv
            ||  drvModelString == "drv8825"sv
            )
        return DrvModel::DRV8825;
    else
    {
        return DrvModel::INVALID;
    }
}

//----------------------------
// M words

template<typename T>
uint32_t getDrvMVal(const T& table, uint32_t index)
{
    const auto& m = table[index];
    uint32_t value = 0;

    /* The table is ordered m[2], m[1], m[0], matching cfg[7:2]. */
    for (const auto& bit : m) {
        value = (value << 1) | bit.m_I;
        value = (value << 1) | bit.m_T;
    }
    return value;
}

struct UstepMode {
    uint8_t mode;
    uint8_t mIndex;
};

static constexpr std::array<UstepMode, 9> drv8434aModes = {{
    {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4},
    {5, 5}, {6, 6}, {7, 7}, {8, 8}
}};

static constexpr std::array<UstepMode, 6> drv8825Modes = {{
    {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}
}};

template<typename Modes, typename Table>
std::optional<uint32_t> getMWord(const Modes& modes, const Table& table,
                                 uint32_t mode)
{
    for (const auto& entry : modes) {
        if (entry.mode == mode)
            return getDrvMVal(table, entry.mIndex);
    }
    return std::nullopt;
}

template<typename Modes, typename Table>
std::optional<uint32_t> getExponent(const Modes& modes, const Table& table,
                                    uint32_t mWord)
{
    for (const auto& mode : modes) {
        if (getDrvMVal(table, mode.mIndex) == mWord)
            return mode.mode;
    }
    return std::nullopt;
}

std::optional<uint32_t>
getDrvMWord(DrvModel drvModel, uint32_t mode)
{
    switch (drvModel) {
        case DRV8434A:
            return getMWord(drv8434aModes, DRV8434A_M, mode);
        case DRV8825:
            return getMWord(drv8825Modes, DRV8825_M, mode);
        default:
            return std::nullopt;
    }
}

std::optional<uint32_t>
getDrvUstepMode(DrvModel drvModel, uint32_t mWord)
{
    switch (drvModel) {
        case DRV8434A:
            return getExponent(drv8434aModes, DRV8434A_M, mWord);
        case DRV8825:
            return getExponent(drv8825Modes, DRV8825_M, mWord);
        default:
            return std::nullopt;
    }
}

}
