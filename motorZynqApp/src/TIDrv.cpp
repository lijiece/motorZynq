#include <TIDrv.hpp>

#include <array>
#include <cstdint>
#include <iostream>
#include <string_view>

using namespace std::string_view_literals;

namespace TIDrv {

DrvModel getDrvModel( const std::string& drvModelString )
{
    if (   drvModelString == "DRV8434A"sv
       ||  drvModelString == "drv8434a"sv
       )
        return DrvModel::DRV8434A;
    else if (   drvModelString == "DRV8434A"sv
            ||  drvModelString == "drv8434a"sv
            )
        return DrvModel::DRV8825;
    else
    {
        std::cout << __func__
	          << ": Invalid driver model " << drvModelString
		  << "!\n";
        exit(EXIT_FAILURE);
    }
}

//----------------------------
// M words

uint32_t getDrvMWord( const DrvModel drvModel
                    , const uint32_t index
		    )
{
    switch( drvModel )
    {
        case DrvModel::DRV8434A:
            return getDrvMVal<decltype(DRV8434A_M)>( DRV8434A_M, index );
	case DrvModel::DRV8825:
            return getDrvMVal<decltype(DRV8434A_M)>( DRV8434A_M, index );
	default:
            std::cout << __func__
	              << ": Invalid driver model " << drvModel
	    	  << "!\n";
            exit(EXIT_FAILURE);
    }    
}

template<typename T>
uint32_t getDrvMVal ( const T& M
                    , const uint32_t index
		    )
{
    if ( index < M.size() )
    {
        const auto& m = M[index];
	uint32_t val = 0;
	for ( auto b : m )
	{
	    val = ( val << 1 ) | b.m_I;
	    val = ( val << 1 ) | b.m_T;
	}
        return val;
    }
    std::cout << __func__
              << ": incorrect index "
	      << index
	      << " when getting M value"
	      << std::endl;
    return -1;
}

}

