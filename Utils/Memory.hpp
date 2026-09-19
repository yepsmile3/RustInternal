#pragma once
#include <Core/Core.hpp>

namespace memory
{
	template<typename t>
	inline auto valid_pointer( t pointer ) -> bool {
		return ( pointer && pointer > ( t ) 0x400000 && pointer < ( t ) 0x7FFFFFFFFFFF );
	}
}